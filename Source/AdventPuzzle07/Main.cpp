#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Print.h"

struct Equation
{
    int64_t mySolution;
    std::vector<int64_t> myVariables;
};

enum class Operator
{
    Add = 0,
    Multiply,
    Combine
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
                eq.mySolution = std::stoll(digit);
                digit.clear();
            }
            else if (c == ' ' && !digit.empty())
            {
                eq.myVariables.push_back(std::stoll(digit));
                digit.clear();
            }
        }

        if (!digit.empty())
        {
            eq.myVariables.push_back(std::stoll(digit));
        }

        outEquations.push_back(eq);
    }

    file.close();

    if (outEquations.empty())
    {
        Debug::Print("outEquations is empty");
    }
}

int64_t CalculateSolution(std::vector<int64_t> someVariables, const std::vector<int64_t>& opCombination)
{
    int64_t sum = 0;
    for (int varIndex = 0; varIndex < someVariables.size() - 1; ++varIndex)
    {
        const int64_t firstNum = someVariables[varIndex];
        const int64_t secondNum = someVariables[varIndex + 1];

        const Operator op = static_cast<Operator>(opCombination[varIndex]);
        switch (op)
        {
            case Operator::Add:
                {
                    someVariables[varIndex + 1] = firstNum + secondNum;
                    break;
                }
            case Operator::Multiply:
                {
                    someVariables[varIndex + 1] = firstNum * secondNum;
                    break;
                }
            case Operator::Combine:
                {
                    const std::string combination = std::to_string(firstNum) + std::to_string(secondNum);
                    someVariables[varIndex + 1] = std::stoll(combination);
                    break;
                }
        }
    }

    return someVariables[someVariables.size() - 1];
}

void FindAllOperatorCombinations(std::vector<std::vector<int64_t>>& outAllCombinations,
                                 const std::vector<Operator>& someActiveOperators, const int aVariableAmount)
{
    const int pairAmount = aVariableAmount - 1;
    const int maxOpCombinations = std::pow(someActiveOperators.size(), pairAmount);
    const int defaultOp = static_cast<int>(Operator::Add);
    const int maxOp = static_cast<int>(someActiveOperators[someActiveOperators.size() - 1]);

    std::vector<int64_t> combination;
    for (int i = 0; i < pairAmount; ++i)
    {
        combination.push_back(defaultOp);
    }

    const int start = 0;
    int it = 0;
    while (it < maxOpCombinations)
    {
        outAllCombinations.push_back(combination);

        int64_t& op = combination[start];
        if (op < maxOp)
        {
            ++op;
        }
        else
        {
            int nextIndex = start + 1;
            bool hasPushed = false;
            while (!hasPushed && nextIndex < combination.size())
            {
                int64_t& next = combination[nextIndex];
                if (next < maxOp)
                {
                    for (int r = 0; r < nextIndex; ++r)
                    {
                        combination[r] = defaultOp;
                    }

                    ++next;
                    hasPushed = true;
                }
                else
                {
                    ++nextIndex;
                }
            }
        }

        ++it;
    }
}

int64_t FindSolutions(const std::vector<Equation>& someEquations, const std::vector<Operator>& someActiveOperators)
{
    int64_t result = 0;
    for (const Equation eq : someEquations)
    {
        std::vector<std::vector<int64_t>> allOpCombinations;
        FindAllOperatorCombinations(allOpCombinations, someActiveOperators, eq.myVariables.size());

        for (const std::vector<int64_t>& opCombination : allOpCombinations)
        {
            const int64_t sum = CalculateSolution(eq.myVariables, opCombination);

            if (sum == eq.mySolution)
            {
                result += eq.mySolution;
                break;
            }
        }
    }

    return result;
}

int64_t Part1(const std::string filePath)
{
    std::vector<Equation> equations;
    ReadFile(equations, filePath);

    std::vector<Operator> operators;
    operators.push_back(Operator::Add);
    operators.push_back(Operator::Multiply);

    return FindSolutions(equations, operators);
}

int64_t Part2(const std::string filePath)
{
    std::vector<Equation> equations;
    ReadFile(equations, filePath);

    std::vector<Operator> operators;
    operators.push_back(Operator::Add);
    operators.push_back(Operator::Multiply);
    operators.push_back(Operator::Combine);

    return FindSolutions(equations, operators);
}

int main()
{
    const std::string filePath = "../../Inputs/puzzle_07_input.txt";
    // const std::string filePath = "../../Inputs/puzzle_07_input_test.txt";

    const int64_t resultPart1 = Part1(filePath);
    Debug::Print(std::to_string(resultPart1));

    // Depending on input size, this part may take a couple of minutes to execute
    // due to the amount of string/long long conversions.
    const int64_t resultPart2 = Part2(filePath);
    Debug::Print(std::to_string(resultPart2));

    return 0;
}
