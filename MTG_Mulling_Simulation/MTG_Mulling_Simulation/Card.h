#pragma once
#include <string>

enum class CardType : unsigned long
{
	None = 0,
	Artifact = 1,
	Creature = 1 << 1,
	Enchantment = 1 << 2,
	Land = 1 << 3,
	Planeswalker = 1 << 4,
	Instant = 1 << 5,
	Sorcery = 1 << 6,
	Legendary = 1 << 7,
	Plains = 1 << 8,
	Island = 1 << 9,
	Swamp = 1 << 10,
	Mountain = 1 << 11,
	Forest = 1 << 12
};

inline static const CardType operator|(const CardType& aLHS, const CardType& aRHS)
{
	return static_cast<CardType>(static_cast<unsigned long>(aLHS) | static_cast<unsigned long>(aRHS));
}

inline static const CardType operator&(const CardType& aLHS, const CardType& aRHS)
{
	return static_cast<CardType>(static_cast<unsigned long>(aLHS) & static_cast<unsigned long>(aRHS));
}

class Card
{
public:
	Card();
	~Card();

	void Init(const std::string& aName, const size_t anID, const CardType& aCardType);
	const size_t GetID() const;
	const CardType GetType() const;
	const std::string& GetName() const;

	const bool IsType(const CardType& aType) const;

private:
	std::string myName;
	size_t myCardID;
	CardType myCardType;
};

