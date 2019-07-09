#pragma once

#include "CardInstance.h"
#include <vector>

class Deck
{
public:
	Deck();
	~Deck();

	void AddCard(const Card& aCard);
	void AddCard(const Card& aCard, const size_t anAmount);

	void RemoveCard(const int anIndex);
	void ReplaceCard(const int anIndex, const Card& aCard);

	void CreatePlayableDeck(std::vector<const CardInstance*>& anOutDecklist) const;

	const size_t Size() const;

private:
	std::vector<CardInstance> myCards;
};

