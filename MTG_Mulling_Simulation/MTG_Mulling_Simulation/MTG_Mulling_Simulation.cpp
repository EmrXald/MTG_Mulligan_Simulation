#include "pch.h"
#include "Conditions.h"
#include "MulliganSimulator.h"
#include <iostream>
#include <thread>
#include <filesystem>
#include "include/rapidjson/document.h"
#include "include/rapidjson/filereadstream.h"

typedef std::function<void()> ThreadWork;

void PrintResults(const MulliganResults& someResults, const std::string& MulliganType)
{
	std::cout << MulliganType << " Mulligan Results: \n";

	for (int i = 0; i < someResults.size(); ++i)
	{
		switch (static_cast<MulliganResult>(i))
		{
		case MulliganResult::Success_7_Hand:
			std::cout << 7 << " hand Keep: " << someResults[i] << "\n";
			break;
		case MulliganResult::Success_6_Hand:
			std::cout << 6 << " hand Keep: " << someResults[i] << "\n";
			break;
		case MulliganResult::Success_5_Hand:
			std::cout << 5 << " hand Keep: " << someResults[i] << "\n";
			break;
		case MulliganResult::Success_4_Hand:
			std::cout << 4 << " hand Keep: " << someResults[i] << "\n";
			break;
		case MulliganResult::Failure:
			std::cout << "Failed to find keepable hand: " << someResults[i] << "\n";
			break;
		}
	}
}

const bool FindCard(const std::vector<Card>& aCardRepo, const std::string& aName,const Card*& anOutCard)
{
	for (const Card& c : aCardRepo)
	{
		if (c.GetName() == aName)
		{
			anOutCard = &c;
			return true;
		}
	}
	anOutCard = nullptr;
	return false;
}

const bool FindCard(const std::vector<Card>& aCardRepo, const size_t& anID, const Card*& anOutCard)
{
	for (const Card& c : aCardRepo)
	{
		if (c.GetID() == anID)
		{
			anOutCard = &c;
			return true;
		}
	}

	anOutCard = nullptr;
	return false;
}

void DoSplitSim(const ThreadWork& someWorkFunc, std::vector<std::thread>& someWorkers)
{
	for (std::thread& t : someWorkers)
	{
		t = std::move(std::thread(someWorkFunc));
	}

	someWorkFunc();

	for (std::thread& t : someWorkers)
	{
		t.join();
	}
}

int main()
{
	Deck edhDeck;

	std::string cardRepoPath;
	std::cout << "Type path to card library: \n";
	std::cin >> cardRepoPath;

	bool fileExists = std::filesystem::exists(cardRepoPath);
	bool validPath = cardRepoPath.find(".json") != std::string::npos;
	
	while ((!validPath) || (!fileExists))
	{
		std::cout << "Bad path, must be a json file. type the path again.\n";
		cardRepoPath.resize(0);
		std::cin >> cardRepoPath;

		fileExists = std::filesystem::exists(cardRepoPath);
		validPath = cardRepoPath.find(".json") != std::string::npos;
	}

	std::cout << "Card library found, parsing...\n";

	FILE* fp;
	fopen_s(&fp, cardRepoPath.c_str(), "rb");

	char libReadBuffer[65535];
	rapidjson::FileReadStream is(fp, libReadBuffer, sizeof(libReadBuffer));

	rapidjson::Document libraryDoc;
	libraryDoc.ParseStream(is);
	fclose(fp);

	std::vector<Card> cardRepo;
	unsigned int cardID = 0;
	for (unsigned int i = 0; i < libraryDoc.Size(); ++i)
	{
		Card toAdd;
		toAdd.Init(libraryDoc[i]["Name"].GetString(), cardID++, static_cast<CardType>(libraryDoc[i]["Type"].GetInt()));
		cardRepo.emplace_back(toAdd);
	}
	cardRepoPath.resize(0);
	std::cout << "Card library loaded. Type deck path to load deck for simulation:\n";
	std::cin >> cardRepoPath;

	fileExists = std::filesystem::exists(cardRepoPath);
	validPath = cardRepoPath.find(".json") != std::string::npos;

	while ((!validPath) || (!fileExists))
	{
		std::cout << "Bad path, must be a json file. type the path again.\n";
		cardRepoPath.resize(0);
		std::cin >> cardRepoPath;

		fileExists = std::filesystem::exists(cardRepoPath);
		validPath = cardRepoPath.find(".json") != std::string::npos;
	}

	std::cout << "Deck found, parsing...\n";

	fopen_s(&fp, cardRepoPath.c_str(), "rb");

	char deckReadBuffer[65535];
	is = rapidjson::FileReadStream(fp, deckReadBuffer, sizeof(deckReadBuffer));
	rapidjson::Document deckDoc;
	deckDoc.ParseStream(is);
	fclose(fp);

	Card failedToFind;
	failedToFind.Init("failed to find", UINT32_MAX, CardType::None);
	for (unsigned int i = 0; i < deckDoc.Size(); ++i)
	{
		const Card* cardToFind = nullptr;
		rapidjson::Value& ele = deckDoc[i];
		bool cardFound = false;
		if (ele.HasMember("Name"))
		{
			cardFound = FindCard(cardRepo, ele["Name"].GetString(), cardToFind);
		}
		else if (ele.HasMember("ID"))
		{
			cardFound = FindCard(cardRepo, ele["ID"].GetUint(), cardToFind);
		}

		size_t quantity = ele.HasMember("Quantity") ? ele["Quantity"].GetUint() : 0;
		if (cardFound)
		{
			edhDeck.AddCard(*cardToFind, quantity);
			continue;
		}
		else
		{
			edhDeck.AddCard(failedToFind, quantity);
		}
	}

	std::cout << "Deck parsing completed.\nType the number of simulations to begin.\nNote that this program will use as many CPU cores as possible\nto speed up the process and the computer will slow down during simulation.\n";
	unsigned int workerCount = std::thread::hardware_concurrency() - 1;
	unsigned int simulations = 1'000'000;
	
	std::cin >> simulations;

	while (!std::cin.good())
	{
		std::cout << "Invalid simulation count input. Please type a positive whole number, e.g. 10000.\n";
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		std::cin >> simulations;
	}
	
	simulations = (unsigned int)((float)simulations / (float)(workerCount + 1));

	MulliganResults londonResults = {0,0,0,0,0,0,0,0};
	MulliganResults partialParisResults = { 0,0,0,0,0,0,0,0 };
	MulliganResults vancouverResults = { 0,0,0,0,0,0,0,0 };

	StartingHandCondition flashHulkCondition = std::bind(FlashHulkCondBlackTutor, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	ThreadWork londonFunc(std::bind(&MulliganSimulator::LondonMulligan, edhDeck, simulations, flashHulkCondition, std::ref(londonResults)));
	ThreadWork parisFunc(std::bind(&MulliganSimulator::PartialParisMulligan, edhDeck, simulations, flashHulkCondition, std::ref(partialParisResults)));
	ThreadWork vancouverFunc(std::bind(&MulliganSimulator::VancouverMulligan, edhDeck, simulations, flashHulkCondition, std::ref(vancouverResults)));

	std::vector<std::thread> workers(workerCount);

	DoSplitSim(londonFunc, workers);
	DoSplitSim(parisFunc, workers);
	DoSplitSim(vancouverFunc, workers);

	PrintResults(londonResults, "London");
	PrintResults(partialParisResults, "Partial Paris");
	PrintResults(vancouverResults, "Vancouver");

	system("pause");
}
