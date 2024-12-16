#include "Print.h"
#include <fstream>
#include <queue>
#include <regex>
#include <string>
#include <vector>

void Part1Parse(const std::string &anInput)
{
    std::string d1, d2, mulCheck;
    char expectedChar = ' ';
    int totalResult = 0;
    int totalMatches = 0;

    auto Reset = [&d1, &d2, &mulCheck, &expectedChar]() -> void {
        d1 = "";
        d2 = "";
        mulCheck = "";
        expectedChar = ' ';
    };

    for (char c : anInput)
    {
        if (mulCheck == "mul")
        {
            if (std::isdigit(c))
            {
                if (expectedChar == ',')
                {
                    d1 += c;
                }
                else if (expectedChar == ')')
                {
                    d2 += c;
                }
            }

            else if (c == expectedChar) // Handle expectedChar
            {
                switch (expectedChar)
                {
                case '(': {
                    expectedChar = ',';
                    break;
                }
                case ',': {
                    expectedChar = ')';
                    break;
                }
                case ')': // Calculate product
                {
                    const int digit1 = std::stoi(d1);
                    const int digit2 = std::stoi(d2);

                    totalResult += digit1 * digit2;

                    ++totalMatches;

                    Reset();

                    break;
                }
                default: {
                    Reset();
                    break;
                }
                }
            }
            else
            {
                Reset();
            }
        }
        else // Build "mul"
        {
            mulCheck += c;
            if (mulCheck == "mul")
            {
                expectedChar = '(';
            }
            else if ((mulCheck == "m" || mulCheck == "mu") == false)
            {
                Reset();
            }
        }
    }
    Debug::Print("Parse Matches:");
    Debug::PrintInt(totalMatches);
    Debug::Print("Parse Result:");
    Debug::PrintInt(totalResult);
}

void Part1Regex(const std::string &anInput)
{
    std::regex wordPattern(R"(mul\((\d+),(\d+)\))");

    std::sregex_iterator wordsBegin = std::sregex_iterator(anInput.begin(), anInput.end(), wordPattern);

    std::sregex_iterator wordsEnd;

    int totalMatches = 0;
    int result = 0;
    for (std::sregex_iterator it = wordsBegin; it != wordsEnd; ++it)
    {
        std::smatch match = *it;
        result += std::stoi(match[1].str()) * std::stoi(match[2].str());
        ++totalMatches;
    }

    Debug::Print("Regex Matches:");
    Debug::PrintInt(totalMatches);
    Debug::Print("Regex Result:");
    Debug::PrintInt(result);
}

void Part2Regex(const std::string &anInput)
{
    std::regex wordPattern(R"((mul\((\d+),(\d+)\))|(do\(\))|(don't\(\)))");

    std::sregex_iterator wordsBegin = std::sregex_iterator(anInput.begin(), anInput.end(), wordPattern);

    std::sregex_iterator wordsEnd;

    int totalMatches = 0;
    int result = 0;

    bool isMulEnabled = true;
    for (std::sregex_iterator it = wordsBegin; it != wordsEnd; ++it)
    {
        std::smatch matches = *it;
        std::string match = matches.str();
        if (match == "do()")
        {
            isMulEnabled = true;
        }
        else if (match == "don't()")
        {
            isMulEnabled = false;
        }
        else if (isMulEnabled)
        {
            result += std::stoi(matches[2].str()) * std::stoi(matches[3].str());
            ++totalMatches;
        }
    }

    Debug::Print("Regex Matches:");
    Debug::PrintInt(totalMatches);
    Debug::Print("Regex Result:");
    Debug::PrintInt(result);
}

int main()
{
    std::string filePath = "../../Inputs/puzzle_03_input.txt";
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        Debug::Print("failed to open " + filePath);
    }

    std::string input;
    std::string inputLine = "";
    while (getline(file, inputLine))
    {
        input += inputLine;
    }

    file.close();

    Part1Parse(input);
    Debug::Print("");
    Part2Regex(input);
}
