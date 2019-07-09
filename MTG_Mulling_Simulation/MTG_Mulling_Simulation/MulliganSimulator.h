#pragma once
#include <array>
#include <atomic>
#include <functional>

class Deck;
class CardInstance;

enum class MulliganResult
{
	Success_7_Hand,
	Success_6_Hand,
	Success_5_Hand,
	Success_4_Hand,
	Success_3_Hand,
	Success_2_Hand,
	Success_1_Hand,
	Failure,
	Count
};

typedef std::array<std::atomic<int>, static_cast<int>(MulliganResult::Count)> MulliganResults;
/*first argument: the hand to evaluate. second argument: what cards(index) to discard in the mulligan.
return value: if the hand is to be kept.*/
typedef std::function<const bool(const std::vector<const CardInstance*>&, const size_t, std::vector<size_t>&)> StartingHandCondition;

class MulliganSimulator
{
public:
	MulliganSimulator();
	~MulliganSimulator();

	static void VancouverMulligan(const Deck& aDeck, const int aNumberOfSimulatons, const StartingHandCondition& aCondition, MulliganResults& someMulliganResultsOut);
	static void PartialParisMulligan(const Deck& aDeck, const int aNumberOfSimulatons, const StartingHandCondition& aCondition, MulliganResults& someMulliganResultsOut);
	static void LondonMulligan(const Deck& aDeck, const int aNumberOfSimulatons, const StartingHandCondition& aCondition, MulliganResults& someMulliganResultsOut);

private:

	static void ShuffleDeck(std::vector<const CardInstance*>& aListOutToShuffle);

	static void DrawCard(std::vector<const CardInstance*>& aLibrary, std::vector<const CardInstance*>& aHandToDrawTo);
	static void ReturnToLibrary(std::vector<const CardInstance*>& aLibrary, std::vector<const CardInstance*>& aHandToDrawTo);
};

