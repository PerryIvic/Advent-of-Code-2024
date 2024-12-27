#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "Vector2.h"

#include "Print.h"

using Map = std::vector<std::vector<char>>;

struct GuardData
{
	Math::Vector2 myPosition;
	Math::Vector2 myDirection;
};

void ReadMap(Map& aMap, const std::string aPath)
{
	std::ifstream file(aPath);

	if (!file.is_open())
	{
		Debug::Print("Failed to open " + aPath);
	}

	std::string line;
	while (getline(file, line))
	{
		std::vector<char> row(line.begin(), line.end());
		aMap.push_back(row);
	}

	file.close();

	if (aMap.empty())
	{
		Debug::Print("Map is empty");
	}
}

GuardData FindGuardData(const Map& aMap)
{
	for (int y = 0; y < aMap.size(); ++y)
	{
		for (int x = 0; x < aMap[y].size(); ++x)
		{
			char tile = aMap[y][x];

			if (tile == '^')
			{
				GuardData spawnData;
				spawnData.myDirection = { 0 , -1 };
				spawnData.myPosition = { x, y };

				return spawnData;
			}
		}
	}
}

bool IsPositionWithinMap(const Math::Vector2 aPosition, const Map& aMap)
{
	if (aPosition.myY >= 0 && aPosition.myY < aMap.size())
	{
		if (aPosition.myX >= 0 && aPosition.myX < aMap[aPosition.myY].size())
		{
			return true;
		}
	}

	return false;
}

void GuardTraversesMap(Map& aMap)
{
	GuardData guard = FindGuardData(aMap);

	bool isGuardOutOfBounds = false;
	while (!isGuardOutOfBounds)
	{
		Math::Vector2 guardPos = guard.myPosition;
		aMap[guardPos.myY][guardPos.myX] = 'X';

		Math::Vector2 aheadPos = guard.myPosition + guard.myDirection;
		if (IsPositionWithinMap(aheadPos, aMap))
		{
			char tile = aMap[aheadPos.myY][aheadPos.myX];
			if (tile == '#')
			{
				guard.myDirection = { -guard.myDirection.myY, guard.myDirection.myX };
			}
			else
			{
				guard.myPosition += guard.myDirection;
			}
		}
		else
		{
			isGuardOutOfBounds = true;
		}
	}
}

int CountTraversedTiles(const Map& aMap)
{
	int numExploredTiles = 0;
	for (int y = 0; y < aMap.size(); ++y)
	{
		for (int x = 0; x < aMap[y].size(); ++x)
		{
			const char tile = aMap[y][x];
			if (tile == 'X')
				++numExploredTiles;
		}
	}

	return numExploredTiles;
}

void PrintMap(const Map& aMap)
{
	for (int y = 0; y < aMap.size(); ++y)
	{
		std::string line;
		for (int x = 0; x < aMap[y].size(); ++x)
		{
			line += aMap[y][x];
		}
		Debug::Print(line);
	}
}

int main()
{
	const std::string mapPath = "../../Inputs/puzzle_06_input.txt";
	//const std::string mapPath = "../../Inputs/puzzle_06_input_test.txt";

	Map map;
	ReadMap(map, mapPath);

	GuardTraversesMap(map);

	PrintMap(map);

	const int result = CountTraversedTiles(map);

	Debug::PrintInt(result);

	return 0;
}