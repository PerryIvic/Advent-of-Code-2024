#include "Print.h"
#include <Vector2.h>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

struct Node
{
    Node() : myFrequency('?'), myHasAntiNode(false) {};

    char myFrequency;
    bool myHasAntiNode;
};

using Grid = std::vector<std::vector<Node>>;

void PrintGrid(const Grid& aGrid)
{
    for (uint32_t y = 0; y < aGrid.size(); ++y)
    {
        for (uint32_t x = 0; x < aGrid[y].size(); ++x)
        {
            std::cout << aGrid[y][x].myFrequency;
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void ReadFile(Grid& outGrid, const std::string aPath)
{
    std::ifstream file(aPath);

    if (!file.is_open())
    {
        Debug::Print("Failed to open " + aPath);
    }

    std::string line;
    while (getline(file, line))
    {
        Node tile;
        std::vector<Node> row;
        for (char c : line)
        {
            tile.myFrequency = c;
            row.push_back(tile);
        }

        outGrid.push_back(row);
    }

    file.close();

    if (outGrid.empty())
    {
        Debug::Print("outGrid is empty");
    }
}

bool IsWithinGrid(const Grid& aGrid, const Math::Vector2 aPosition)
{
    if (aPosition.myY >= 0 && aPosition.myY < aGrid.size())
    {
        if (aPosition.myX >= 0 && aPosition.myX < aGrid[aPosition.myY].size())
        {
            return true;
        }
    }

    return false;
}

bool IsSatelite(const char anId)
{
    return anId != '.' && anId != '#';
}

void RegisterAntiNodes(Grid& aGrid, const bool aShouldUseResonantHarmonics)
{
    for (int innerY = 0; innerY < aGrid.size(); ++innerY)
    {
        for (int innerX = 0; innerX < aGrid[innerY].size(); ++innerX)
        {
            const Node firstSatelite = aGrid[innerY][innerX];
            if (!IsSatelite(firstSatelite.myFrequency))
                continue;

            for (int outerY = 0; outerY < aGrid.size(); ++outerY)
            {
                for (int outerX = 0; outerX < aGrid[outerY].size(); ++outerX)
                {
                    const Node secondSatelite = aGrid[outerY][outerX];

                    const Math::Vector2 innerPos = Math::Vector2(innerX, innerY);
                    const Math::Vector2 outerPos = Math::Vector2(outerX, outerY);
                    if (!IsSatelite(secondSatelite.myFrequency) ||
                        firstSatelite.myFrequency != secondSatelite.myFrequency || innerPos == outerPos)
                        continue;

                    const Math::Vector2 diff = (outerPos - innerPos);
                    Math::Vector2 antiNodePos = outerPos;

                    if (aShouldUseResonantHarmonics)
                    {
                        while (IsWithinGrid(aGrid, antiNodePos))
                        {
                            Node& antiNode = aGrid[antiNodePos.myY][antiNodePos.myX];
                            if (!IsSatelite(antiNode.myFrequency))
                            {
                                antiNode.myFrequency = '#';
                            }
                            antiNode.myHasAntiNode = true;

                            antiNodePos += diff;
                        }
                    }
                    else
                    {
                        antiNodePos += diff;
                        if (IsWithinGrid(aGrid, antiNodePos))
                        {
                            Node& antiNode = aGrid[antiNodePos.myY][antiNodePos.myX];
                            if (!IsSatelite(antiNode.myFrequency))
                            {
                                antiNode.myFrequency = '#';
                            }
                            antiNode.myHasAntiNode = true;
                        }
                    }
                }
            }
        }
    }
}

int GetUniqueAntiNodeAmount(const Grid& aGrid)
{
    int antiNodeAmount = 0;
    for (int innerY = 0; innerY < aGrid.size(); ++innerY)
    {
        for (int innerX = 0; innerX < aGrid[innerY].size(); ++innerX)
        {
            const Node firstTile = aGrid[innerY][innerX];
            if (firstTile.myHasAntiNode)
            {
                ++antiNodeAmount;
            }
        }
    }

    return antiNodeAmount;
}

int Solution(const std::string aFilePath, const bool aShouldUseResonantHarmonics)
{
    Grid grid;
    ReadFile(grid, aFilePath);

    RegisterAntiNodes(grid, aShouldUseResonantHarmonics);

    PrintGrid(grid);

    return GetUniqueAntiNodeAmount(grid);
}

int main()
{
    const std::string filePath = "../../Inputs/puzzle_08_input.txt";
    // const std::string filePath = "../../Inputs/puzzle_08_test_input_01.txt";
    // const std::string filePath = "../../Inputs/puzzle_08_test_input_02.txt";

    const int resultPart1 = Solution(filePath, false);
    Debug::PrintInt(resultPart1);

    const int resultPart2 = Solution(filePath, true);
    Debug::PrintInt(resultPart2);

    return 0;
}
