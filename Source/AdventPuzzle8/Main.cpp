#include "Print.h"
#include <Vector2.h>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

struct Tile
{
    Tile() : myID('?'), myHasAntiNode(false) {};

    char myID;
    bool myHasAntiNode;
};

using Grid = std::vector<std::vector<Tile>>;

void PrintGrid(const Grid& aGrid)
{
    for (uint32_t y = 0; y < aGrid.size(); ++y)
    {
        for (uint32_t x = 0; x < aGrid[y].size(); ++x)
        {
            std::cout << aGrid[y][x].myID;
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
        Tile tile;
        std::vector<Tile> row;
        for (char c : line)
        {
            tile.myID = c;
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

void RegisterAntiNodes(Grid& aGrid)
{
    for (int innerY = 0; innerY < aGrid.size(); ++innerY)
    {
        for (int innerX = 0; innerX < aGrid[innerY].size(); ++innerX)
        {
            const Tile firstTile = aGrid[innerY][innerX];
            if (!IsSatelite(firstTile.myID))
                continue;

            for (int outerY = 0; outerY < aGrid.size(); ++outerY)
            {
                for (int outerX = 0; outerX < aGrid[outerY].size(); ++outerX)
                {
                    const Tile secondTile = aGrid[outerY][outerX];

                    const Math::Vector2 innerPos = Math::Vector2(innerX, innerY);
                    const Math::Vector2 outerPos = Math::Vector2(outerX, outerY);
                    if (!IsSatelite(secondTile.myID) || firstTile.myID != secondTile.myID || innerPos == outerPos)
                        continue;

                    const Math::Vector2 diff = (outerPos - innerPos);

                    const Math::Vector2 antiNodePosInner = innerPos - diff;
                    const Math::Vector2 antiNodePosOuter = outerPos + diff;

                    if (IsWithinGrid(aGrid, antiNodePosInner))
                    {
                        Tile& n1 = aGrid[antiNodePosInner.myY][antiNodePosInner.myX];
                        if (!IsSatelite(n1.myID))
                        {
                            n1.myID = '#';
                        }
                        n1.myHasAntiNode = true;
                    }

                    if (IsWithinGrid(aGrid, antiNodePosOuter))
                    {
                        Tile& n2 = aGrid[antiNodePosOuter.myY][antiNodePosOuter.myX];
                        if (!IsSatelite(n2.myID))
                        {
                            n2.myID = '#';
                        }
                        n2.myHasAntiNode = true;
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
            const Tile firstTile = aGrid[innerY][innerX];
            if (firstTile.myHasAntiNode)
            {
                ++antiNodeAmount;
            }
        }
    }

    return antiNodeAmount;
}

int Part1(const std::string aFilePath)
{
    Grid grid;
    ReadFile(grid, aFilePath);

    RegisterAntiNodes(grid);

    PrintGrid(grid);

    return GetUniqueAntiNodeAmount(grid);
}

int main()
{
    const std::string filePath = "../../Inputs/puzzle_08_input.txt";
    // const std::string filePath = "../../Inputs/puzzle_08_test_input_01.txt";
    // const std::string filePath = "../../Inputs/puzzle_08_test_input_02.txt";

    const int resultPart1 = Part1(filePath);

    Debug::PrintInt(resultPart1);

    return 0;
}
