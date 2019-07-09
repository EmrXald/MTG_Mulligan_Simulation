#include "pch.h"
#include "Deck.h"
#include "Card.h"

Deck::Deck()
{
}


Deck::~Deck()
{
}

void Deck::AddCard(const Card & aCard)
{
	CardInstance cardToAdd;
	cardToAdd.Init(aCard);
	myCards.push_back(cardToAdd);
}

void Deck::AddCard(const Card & aCard, const size_t anAmount)
{
	for (size_t i = 0; i < anAmount; ++i)
	{
		AddCard(aCard);
	}
}

void Deck::RemoveCard(const int anIndex)
{
	myCards.erase(myCards.begin() + anIndex);
}

void Deck::ReplaceCard(const int anIndex, const Card & aCard)
{
	CardInstance cardToAdd;
	cardToAdd.Init(aCard);
	myCards[anIndex] = cardToAdd;
}

void Deck::CreatePlayableDeck(std::vector<const CardInstance*>& anOutDecklist) const
{
	anOutDecklist.reserve(myCards.size());
	anOutDecklist.resize(0);

	for (const CardInstance& card : myCards)
	{
		anOutDecklist.push_back(&card);
	}

}

const size_t Deck::Size() const
{
	return myCards.size();
}
