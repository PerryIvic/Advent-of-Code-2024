#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "Print.h"

using Grid = std::vector<std::vector<char>>;

void ReadMap(Grid& aGrid, const std::string aPath)
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
		aGrid.push_back(row);
	}

	file.close();
}

int main()
{
	const std::string mapPath = "../../Inputs/puzzle_06_input_test";

	Grid grid;
	ReadMap(grid, mapPath);

	return 0;
}