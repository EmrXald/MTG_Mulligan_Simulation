#include "pch.h"
#include "CardInstance.h"


CardInstance::CardInstance()
{
	myCardBlueprint = nullptr;
}


CardInstance::~CardInstance()
{
	myCardBlueprint = nullptr;
}

void CardInstance::Init(const Card & aBlueprint)
{
	myCardBlueprint = &aBlueprint;
}

const Card * CardInstance::GetBlueprint() const
{
	return myCardBlueprint;
}
