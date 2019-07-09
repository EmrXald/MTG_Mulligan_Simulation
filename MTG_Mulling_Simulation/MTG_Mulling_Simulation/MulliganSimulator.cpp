#include "pch.h"
#include "MulliganSimulator.h"
#include "Deck.h"

#include <random>
#include <chrono>
#include <algorithm>
#define MINIMUM_HAND 1
MulliganSimulator::MulliganSimulator()
{
}


MulliganSimulator::~MulliganSimulator()
{
}
/*
1. Contruct a library(a collection of references to the cards of the deck).
2. Draw a hand and evaluate if it an acceptable hand via the aCondition variable.
3. If the hand meets the condition, increment and start the next simulation.
4. Else mulligan and draw a new hand with one less card.
*/
void MulliganSimulator::VancouverMulligan(const Deck & aDeck, const int aNumberOfSimulatons, const StartingHandCondition & aCondition, MulliganResults & someMulliganResultsOut)
{
	std::vector<const CardInstance*> library;

	std::vector<const CardInstance*> hand;
	std::vector<size_t> cardsToDiscard;

	aDeck.CreatePlayableDeck(library);

	for (size_t simulationIt = 0; simulationIt < aNumberOfSimulatons; ++simulationIt)
	{
		size_t nmbrOfMulligans = 0;
		size_t startingHandSize = 7;
		bool foundDesiredHand = false;
		while (startingHandSize > MINIMUM_HAND)
		{
			cardsToDiscard.resize(0);
			ShuffleDeck(library);

			for (size_t i = 0; i < startingHandSize; ++i)
			{
				DrawCard(library, hand);
			}

			foundDesiredHand = aCondition(hand, startingHandSize, cardsToDiscard);

			if (foundDesiredHand)
			{
				break;
			}

			++nmbrOfMulligans;
			--startingHandSize;

			ReturnToLibrary(library, hand);
		}

		ReturnToLibrary(library, hand);

		if (!foundDesiredHand)
		{
			++someMulliganResultsOut[static_cast<int>(MulliganResult::Failure)];
		}
		else
		{
			++someMulliganResultsOut[nmbrOfMulligans];
		}	
	}
}

/*
1. Contruct a library(a collection of references to the cards of the deck).
2. Draw a hand and evaluate if it an acceptable hand via the aCondition variable.
3. If the hand meets the condition, increment and start the next simulation.
4. Else store the discarded cards and draw cards from the library until one less than before.
5. Add the discarded cards to the library and shuffle.
*/

void MulliganSimulator::PartialParisMulligan(const Deck & aDeck, const int aNumberOfSimulatons, const StartingHandCondition & aCondition, MulliganResults & someMulliganResultsOut)
{
	std::vector<const CardInstance*> library;

	std::vector<const CardInstance*> hand;
	std::vector<size_t> cardsToDiscard;
	std::vector<const CardInstance*> discardedCards;

	aDeck.CreatePlayableDeck(library);

	for (size_t simulationIt = 0; simulationIt < aNumberOfSimulatons; ++simulationIt)
	{
		size_t nmbrOfMulligans = 0;
		size_t startingHandSize = 7;
		bool foundDesiredHand = false;

		cardsToDiscard.resize(0);
		ShuffleDeck(library);

		while (startingHandSize > MINIMUM_HAND)
		{
			while (hand.size() < startingHandSize)
			{
				DrawCard(library, hand);
			}

			ReturnToLibrary(library, discardedCards);
			ShuffleDeck(library);

			foundDesiredHand = aCondition(hand, startingHandSize, cardsToDiscard);

			if (foundDesiredHand)
			{
				break;
			}
			std::sort(cardsToDiscard.begin(), cardsToDiscard.end(), [&](size_t aLHS, size_t aRHS) {return aLHS > aRHS; });
			for (size_t i = 0; i < cardsToDiscard.size(); ++i)
			{
				size_t cardIndexToDiscard = cardsToDiscard[i];
				discardedCards.push_back(hand[cardIndexToDiscard]);
				hand.erase(hand.begin() + cardIndexToDiscard);
			}

			cardsToDiscard.resize(0);
			++nmbrOfMulligans;
			--startingHandSize;
		}
		
		ReturnToLibrary(library, hand);

		if (!foundDesiredHand)
		{
			++someMulliganResultsOut[static_cast<int>(MulliganResult::Failure)];
		}
		else
		{
			++someMulliganResultsOut[nmbrOfMulligans];
		}
	}

}

void MulliganSimulator::LondonMulligan(const Deck & aDeck, const int aNumberOfSimulatons, const StartingHandCondition & aCondition, MulliganResults & someMulliganResultsOut)
{
	std::vector<const CardInstance*> library;

	std::vector<const CardInstance*> hand;
	std::vector<size_t> cardsToDiscard;

	aDeck.CreatePlayableDeck(library);

	for (size_t simulationIt = 0; simulationIt < aNumberOfSimulatons; ++simulationIt)
	{
		size_t nmbrOfMulligans = 0;
		size_t startingHandSize = 7;
		bool foundDesiredHand = false;
		while (startingHandSize > MINIMUM_HAND)
		{
			cardsToDiscard.resize(0);
			ShuffleDeck(library);

			for (size_t i = 0; i < 7; ++i)
			{
				DrawCard(library, hand);
			}

			foundDesiredHand = aCondition(hand, startingHandSize, cardsToDiscard);

			if (foundDesiredHand)
			{
				break;
			}

			++nmbrOfMulligans;
			--startingHandSize;

			ReturnToLibrary(library, hand);
		}

		ReturnToLibrary(library, hand);

		if (!foundDesiredHand)
		{
			++someMulliganResultsOut[static_cast<int>(MulliganResult::Failure)];
		}
		else
		{
			++someMulliganResultsOut[nmbrOfMulligans];
		}
	}

}

void MulliganSimulator::ShuffleDeck(std::vector<const CardInstance*>& aListOutToShuffle)
{
	long long seed = std::chrono::system_clock::now().time_since_epoch().count();

	shuffle(aListOutToShuffle.begin(), aListOutToShuffle.end(), std::default_random_engine(static_cast<unsigned int>(seed)));
}

void MulliganSimulator::DrawCard(std::vector<const CardInstance*>& aLibrary, std::vector<const CardInstance*>& aHandToDrawTo)
{
	const CardInstance* drawnCard = aLibrary[0];
	aLibrary.erase(aLibrary.begin());
	aHandToDrawTo.push_back(drawnCard);
}

void MulliganSimulator::ReturnToLibrary(std::vector<const CardInstance*>& aLibrary, std::vector<const CardInstance*>& aHandToDrawTo)
{
	for (const CardInstance* card : aHandToDrawTo)
	{
		aLibrary.push_back(card);
	}

	aHandToDrawTo.clear();
}
