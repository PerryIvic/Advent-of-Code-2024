#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "Print.h"

struct Equation
{
	int mySolution;
	std::vector<int> myVariables;
};

enum class Operator
{
	Add = 0,
	Multiply
};

void ReadFile(std::vector<Equation>& outEquations, const std::string aPath)
{
	std::ifstream file(aPath);

	if (!file.is_open())
	{
		Debug::Print("Failed to open " + aPath);
	}

	std::string line;
	while (getline(file, line))
	{
		Equation eq;
		std::string digit;
		for (const char c : line)
		{
			if (std::isdigit(c))
			{
				digit += c;
			}
			else if (c == ':')
			{
				eq.mySolution = std::stoi(digit);
				digit.clear();
			}
			else if (c == ' ' && !digit.empty())
			{
				eq.myVariables.push_back(std::stoi(digit));
				digit.clear();
			}
		}

		if (!digit.empty())
		{
			eq.myVariables.push_back(std::stoi(digit));
		}

		outEquations.push_back(eq);
	}

	file.close();

	if (outEquations.empty())
	{
		Debug::Print("outEquations is empty");
	}
}

int CalculateSolution(const std::vector<int>& someVariables, const std::vector<int>& someOperators)
{
	int sum = 0;
	for (int varIndex = 0; varIndex < someVariables.size() - 1; ++varIndex)
	{
		const int firstNum = someVariables[varIndex];
		const int secondNum = someVariables[varIndex];

		const Operator op = static_cast<Operator>(someOperators[varIndex]);
		switch (op)
		{
		case Operator::Add:
		{
			sum += firstNum + secondNum;
			break;
		}
		case Operator::Multiply:
		{
			sum += firstNum * secondNum;
			break;
		}
		}
	}

	return sum;
}

void FindAllOperatorCombinations(std::vector<std::vector<int>>& outAllCombinations, const std::vector<Operator>& someActiveOperators, const int aVariableAmount)
{
	const int varPairAmount = aVariableAmount - 1;
	const int maxOpCombinations = std::pow(someActiveOperators.size(), varPairAmount);

	const int baseOperator = static_cast<int>(someActiveOperators[0]);
	std::vector<int> opCombination;
	for (int i = 0; i < varPairAmount; ++i)
	{
		opCombination.push_back(baseOperator);
	}

	outAllCombinations.push_back(opCombination);

	for ()
	{

	}
}

int FindPotentialSolutions(const std::vector<Equation>& someEquations, const std::vector<Operator>& someActiveOperators)
{
	int result = 0;
	for (const Equation eq : someEquations)
	{
		std::vector<std::vector<int>> allOpCombinations;
		FindAllOperatorCombinations(allOpCombinations, someActiveOperators, eq.myVariables.size());

		for(const std::vector<int>& opCombination : allOpCombinations)
		{
			int sum = CalculateSolution(eq.myVariables, opCombination);

			if (sum == eq.mySolution)
			{
				result += eq.mySolution;
				break;
			}
		}
	}

	return result;
}

int Part1(const std::string filePath)
{
	std::vector<Equation> equations;
	ReadFile(equations, filePath);

	std::vector<Operator> operators;
	operators.push_back(Operator::Add);
	operators.push_back(Operator::Multiply);

	return FindPotentialSolutions(equations, operators);
}

int main()
{
	//const std::string filePath = "../../Inputs/puzzle_07_input.txt";
	const std::string filePath = "../../Inputs/puzzle_07_input_test.txt";

	//for (int i = 0; i < 10; ++i)
	//{
	//	const int currentIndex = i / 2;
	//	Debug::PrintInt(currentIndex);
	//}


	const int resultPart1 = Part1(filePath);

	return 0;
}