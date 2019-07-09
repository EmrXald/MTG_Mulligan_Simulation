#pragma once

class Card;

class CardInstance
{
public:
	CardInstance();
	~CardInstance();

	void Init(const Card& aBlueprint);
	const Card* GetBlueprint() const;
private:
	const Card* myCardBlueprint;
};

