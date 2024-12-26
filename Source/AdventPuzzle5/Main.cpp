#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "Print.h"

using VectorInt = std::vector<int>;
using Rules = std::unordered_map<int, VectorInt>;
using Updates = std::vector<VectorInt>;

void ReadRules(Rules& outRules, const std::string aPath)
{
	std::ifstream file(aPath);

	if (!file.is_open())
	{
		Debug::Print("Failed to open " + aPath);
	}

	std::string line;
	while (getline(file, line))
	{
		int key = -1;
		std::string num;
		for (const char letter : line)
		{
			if (std::isdigit(letter))
			{
				num += letter;
			}
			else if ('|')
			{
				key = std::stoi(num);
				num = "";
			}
		}

		outRules[key].push_back(std::stoi(num));

	}

	file.close();
}

void ReadUpdates(Updates& outUpdates, const std::string aPath)
{
	std::ifstream file(aPath);

	if (!file.is_open())
	{
		Debug::Print("Failed to open " + aPath);
	}

	VectorInt update;
	std::string line;
	while (getline(file, line))
	{
		std::string num;
		for (const char letter : line)
		{
			if (std::isdigit(letter))
			{
				num += letter;
			}
			else if (',')
			{
				update.push_back(std::stoi(num));
				num = "";
			}
		}
		update.push_back(std::stoi(num));

		outUpdates.push_back(update);
		update.clear();
	}

	file.close();
}

void GetActiveRulesForUpdate(Rules& outActiveRules, const Rules& someRules, const VectorInt& anUpdate)
{
	for (const int key : anUpdate)
	{
		if (someRules.find(key) == someRules.end())
			continue;
		
		const auto ruleSet = someRules.at(key);
		for (const int rule : ruleSet)
		{
			for (const int update : anUpdate)
			{
				if (update == rule)
				{
					outActiveRules[key].push_back(rule);
				}
			}
		}
	}
}

bool IsUpdateFollowingRules(const VectorInt& anUpdate, const Rules& someRules)
{
	for (int keyIndex = 0; keyIndex < anUpdate.size(); ++keyIndex)
	{
		const int key = anUpdate[keyIndex];
		if (someRules.find(key) != someRules.end())
		{
			const auto ruleSet = someRules.at(key);
			for (const int rule : ruleSet)
			{
				for (int ruleIndex = 0; ruleIndex < anUpdate.size(); ++ruleIndex)
				{
					const int ruleTest = anUpdate[ruleIndex];
					if(ruleTest == rule)
					{
						if (keyIndex > ruleIndex)
						{
							return false;
						}
						break;
					}
				}
			}
		}
	}

	return true;
}

void InsertThenShiftElementsRight(VectorInt& anUpdate, const int anIndexToInsert, const int aDestinationIndex)
{
	int previousValue = anUpdate[aDestinationIndex];
	anUpdate[aDestinationIndex] = anUpdate[anIndexToInsert];

	for (int index = aDestinationIndex + 1; index <= anIndexToInsert; ++index)
	{
		const int temp = previousValue;
		previousValue = anUpdate[index];
		anUpdate[index] = temp;
	}
}

void Sort(const Rules& someRules, VectorInt& anUpdate)
{
	constexpr int defaultEmplaceIndex = 9999;
	for (int keyIndex = 0; keyIndex < anUpdate.size(); ++keyIndex)
	{
		const int key = anUpdate[keyIndex];
		if (someRules.find(key) == someRules.end())
			continue;

		int targetIndex = defaultEmplaceIndex;
		const VectorInt& ruleSet = someRules.at(key);
		for (const int& rule : ruleSet)
		{
			for (int ruleIndex = 0; ruleIndex < anUpdate.size(); ++ruleIndex)
			{
				const int ruleTest = anUpdate[ruleIndex];
				if (ruleTest == rule)
				{
					if (keyIndex > ruleIndex && ruleIndex < targetIndex)
					{
						targetIndex = ruleIndex;
						break;
					}
				}
			}
		}

		if (targetIndex != defaultEmplaceIndex)
		{
			InsertThenShiftElementsRight(anUpdate, keyIndex, targetIndex);
		}
	}
}

int ApplyRulesToUpdates(const Rules& someRules, Updates& someUpdates)
{
	int result = 0;
	for (VectorInt& update : someUpdates)
	{
		Rules activeRules;
		GetActiveRulesForUpdate(activeRules, someRules, update);

		if (IsUpdateFollowingRules(update, activeRules))
		{
			continue;
		}

		while(!IsUpdateFollowingRules(update, activeRules))
		{
			Sort(activeRules, update);
		}

		const int middleIndex = (update.size() - 1) / 2;
		result += update[middleIndex];
	}

	return result;
}

int FindCorrectlyOrderedUpdates(const Rules& someRules, const Updates& someUpdates)
{
	int result = 0;
	for (const VectorInt& update : someUpdates)
	{
		Rules activeRules;
		GetActiveRulesForUpdate(activeRules, someRules, update);

		if (IsUpdateFollowingRules(update, activeRules))
		{
			const int middleIndex = (update.size() - 1) / 2;
			result += update[middleIndex];
		}
	}

	return result;
}

int main()
{
	const std::string rulesPath = "../../Inputs/puzzle_05_rules.txt";
	const std::string inputPath = "../../Inputs/puzzle_05_input.txt";
	
	Rules rules;
	ReadRules(rules, rulesPath);

	Updates updates;
	ReadUpdates(updates, inputPath);

	int resultPart1 = FindCorrectlyOrderedUpdates(rules, updates); // Part 1
	int resultPart2 = ApplyRulesToUpdates(rules, updates); // Part 2

	Debug::PrintInt(resultPart1);
	Debug::PrintInt(resultPart2);

	return 0;
}