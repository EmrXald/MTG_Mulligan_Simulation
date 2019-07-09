#pragma once
#include "Deck.h"
#include "Card.h"

enum class ConditionType
{
	CardType,
	CardID,
	Count
};

struct KeepCardCondition
{
	ConditionType myConditionType;
	size_t myDesiredCardID;
	CardType myDesiredCardType;
};

inline const bool FlashHulkCondition(const std::vector<const CardInstance*>& aStartingHand, const size_t aHandSizeAfterMulligan, std::vector<size_t>& someCardsToDiscardOut)
{
	bool hulkInHand = false;
	bool flashInHand = false;
	bool swampInHand = false;
	bool islandInHand = false;
	bool sacOutletInHand = false;
	int landCount = 0;

	for (size_t i = 0; i < aStartingHand.size(); ++i)
	{
		const CardInstance& currentCard = *aStartingHand[i];
		const Card* card = currentCard.GetBlueprint();

		bool shouldBeDiscarded = true;

		if (card->GetName() == "Protean Hulk" && !hulkInHand)
		{
			hulkInHand = true;
			shouldBeDiscarded = false;
		}
		if (card->GetName() == "Flash" && !flashInHand)//Flash ID.
		{
			flashInHand = true;
			shouldBeDiscarded = false;
		}
		if (card->GetName() == "Viscera Seer" && !sacOutletInHand)
		{
			sacOutletInHand = true;
			shouldBeDiscarded = false;
		}
		if (card->IsType(CardType::Island) && !islandInHand)
		{
			islandInHand = true;
			shouldBeDiscarded = false;
		}
		if (card->IsType(CardType::Swamp) && !swampInHand)
		{
			swampInHand = true;
			shouldBeDiscarded = false;
		}
		if (card->IsType(CardType::Land) && (landCount < 2))
		{
			landCount++;
			shouldBeDiscarded = false;
		}

		if (shouldBeDiscarded)
		{
			someCardsToDiscardOut.push_back(i);
		}
	}

	return sacOutletInHand && flashInHand && hulkInHand && islandInHand && swampInHand && (landCount >= 2) && (aHandSizeAfterMulligan >= 5);
}

inline const bool FlashHulkCondBlackTutor(const std::vector<const CardInstance*>& aStartingHand, const size_t aHandSizeAfterMulligan, std::vector<size_t>& someCardsToDiscardOut)
{
	bool hulkInHand = false;
	bool flashInHand = false;
	bool swampInHand = false;
	bool islandInHand = false;
	bool sacOutletInHand = false;
	int landCount = 0;
	int comboCount = 0;

	for (size_t i = 0; i < aStartingHand.size(); ++i) 
	{
		const CardInstance& currentCard = *aStartingHand[i];
		const Card* card = currentCard.GetBlueprint();

		bool shouldBeDiscarded = true;

		if (card->GetName() == "Protean Hulk" && !hulkInHand)
		{
			hulkInHand = true;
			shouldBeDiscarded = false;
			comboCount++;
		}
		if (card->GetName() == "Flash" && !flashInHand)//Flash ID.
		{
			flashInHand = true;
			shouldBeDiscarded = false;
			comboCount++;
		}
		if (card->GetName() == "Viscera Seer" && !sacOutletInHand)
		{
			sacOutletInHand = true;
			shouldBeDiscarded = false;
			comboCount++;
		}
		if (card->IsType(CardType::Island) && !islandInHand)
		{
			islandInHand = true;
			shouldBeDiscarded = false;
		}
		if (card->IsType(CardType::Swamp) && !swampInHand)
		{
			swampInHand = true;
			shouldBeDiscarded = false;
		}
		if (card->IsType(CardType::Land) && (landCount < 2))
		{
			landCount++;
			shouldBeDiscarded = false;
		}

		if (shouldBeDiscarded)
		{
			someCardsToDiscardOut.push_back(i);
		}
	}

	bool hasPerfectHand = sacOutletInHand && flashInHand && hulkInHand && islandInHand && swampInHand && (landCount >= 2) && (aHandSizeAfterMulligan >= 5);
	int tutorCount = 0;
	if (comboCount < 3)//if we do not have the full combo check if any of the cards we have discarded are tutors that make up for it and keep them.
	{
		for (size_t i = 0; i < someCardsToDiscardOut.size(); ++i)
		{
			const size_t discardIndex = someCardsToDiscardOut[i];
			const CardInstance* cardToDiscard = aStartingHand[discardIndex];
			const Card* bp = cardToDiscard->GetBlueprint();
			if (bp->GetName() == "Tutor")
			{
				tutorCount++;
				someCardsToDiscardOut.erase(someCardsToDiscardOut.begin() + i);
				--i;

				if ((tutorCount + comboCount) > 2)
				{
					break;
				}
			}
		}
	}

	bool hasKeepableTutorHand = ((tutorCount + comboCount) > 2) && islandInHand && swampInHand && (landCount >= 2) && (aHandSizeAfterMulligan >= 5);

	return hasPerfectHand || hasKeepableTutorHand;
}