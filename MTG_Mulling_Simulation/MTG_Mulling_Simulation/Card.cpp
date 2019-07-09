#include "pch.h"
#include "Card.h"
#include <algorithm>

Card::Card()
{
}


Card::~Card()
{
}

void Card::Init(const std::string & aName, const size_t anID, const CardType& aCardType)
{
	myName = aName;
	myCardID = anID;
	myCardType = aCardType;
}

const size_t Card::GetID() const
{
	return myCardID;
}

const CardType Card::GetType() const
{
	return myCardType;
}

const std::string & Card::GetName() const
{
	return myName;
}

const bool Card::IsType(const CardType & aType) const
{
	return static_cast<bool>(myCardType & aType);
}
