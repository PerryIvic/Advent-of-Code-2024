#include <fstream>
#include <string>
#include <vector>

#include "Print.h"

void ReadRules(const std::string aPath)
{
	std::ifstream rulesFile(aPath);

	if (!rulesFile.is_open())
	{
		Debug::Print("Failed to open " + aPath);
	}

}

int main()
{
	const std::string rulesPath = "../../Inputs/puzzle_05_rules.txt";
	const std::string inputPath = "../../Inputs/puzzle_05_input.txt";
	
	ReadRules(rulesPath);

	return 0;
}