#include "Print.h"
#include "Report.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

int main()
{
    std::string filePath = "../../Inputs/puzzle_02_input.txt";
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        Debug::Print("failed to open " + filePath);
    }

    std::vector<Report> reports;

    bool hasStoredDigit = false;
    std::string line = "";
    while (getline(file, line))
    {
        std::vector<int> levels;
        std::string strNum;
        for (int i = 0; i < line.size(); ++i)
        {
            char digit = line[i];
            if (std::isdigit(digit))
            {
                strNum += digit;
                hasStoredDigit = true;
            }
            else
            {
                if (hasStoredDigit)
                {
                    levels.push_back(std::stoi(strNum));
                    hasStoredDigit = false;
                    strNum = "";
                }
            }
        }

        if (hasStoredDigit)
        {
            levels.push_back(std::stoi(strNum));
        }
        reports.push_back(Report(levels));

        line = "";
    }

    file.close();

    int numSafeLevels = 0;
    for (auto &level : reports)
    {
        if (level.AreAllLevelsSafe(true))
        {
            ++numSafeLevels;
        }
    }

    Debug::PrintInt(numSafeLevels);
}
