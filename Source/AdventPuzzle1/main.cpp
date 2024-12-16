#include "Print.h"
#include <fstream>
#include <queue>
#include <string>
#include <vector>

void Part1()
{
    std::string fileName = "../../Inputs/puzzle_01_input.txt";
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        Debug::Print("failed to open " + fileName);
        return;
    }

    std::priority_queue<int, std::vector<int>, std::greater<int>> leftList;
    std::priority_queue<int, std::vector<int>, std::greater<int>> rightList;

    std::string line = "";
    while (getline(file, line))
    {
        bool isLeftList = true;
        std::string strNum;
        for (int i = 0; i < line.size(); ++i)
        {
            char digit = line[i];
            if (std::isdigit(digit))
            {
                strNum += digit;
            }
            else
            {
                if (isLeftList)
                {
                    isLeftList = false;
                    leftList.push(std::stoi(strNum));
                    strNum = "";
                }
            }
        }
        rightList.push(std::stoi(strNum));

        line = "";
    }

    file.close();

    int totalDistance = 0;
    while (!leftList.empty() && !rightList.empty())
    {
        const int leftNum = leftList.top();
        const int rightNum = rightList.top();

        totalDistance += std::abs(leftNum - rightNum);

        rightList.pop();
        leftList.pop();
    }

    Debug::PrintInt(totalDistance);
}

void Part2()
{
    std::string fileName = "../../Inputs/puzzle_01_input.txt";
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        Debug::Print("failed to open " + fileName);
        return;
    }

    std::vector<int> leftList;
    std::vector<int> rightList;

    std::string line = "";
    while (getline(file, line))
    {
        bool isLeftList = true;
        std::string strNum;
        for (int i = 0; i < line.size(); ++i)
        {
            char digit = line[i];
            if (std::isdigit(digit))
            {
                strNum += digit;
            }
            else
            {
                if (isLeftList)
                {
                    isLeftList = false;
                    leftList.push_back(std::stoi(strNum));
                    strNum = "";
                }
            }
        }
        rightList.push_back(std::stoi(strNum));

        line = "";
    }

    file.close();

    int similarityScore = 0;
    for (int leftNum : leftList)
    {
        int multiplier = 0;
        for (int rightNum : rightList)
        {
            if (rightNum == leftNum)
            {
                ++multiplier;
            }
        }
        similarityScore += leftNum * multiplier;
    }

    Debug::PrintInt(similarityScore);
}

int main()
{
    Part1();
    Part2();

    return 0;
}
