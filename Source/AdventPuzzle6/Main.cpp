#include "Vector2.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Print.h"

using Map = std::vector<std::vector<char>>;

struct Point
{
    Point() : myPosition(Math::Vector2()), myDirection(Math::Vector2()), myTimesVisited(0)
    {
    }

    Point(Math::Vector2 aPosition, Math::Vector2 aDirection)
        : myPosition(aPosition), myDirection(aDirection), myTimesVisited(0)
    {
    }

    Math::Vector2 myPosition;
    Math::Vector2 myDirection;
    int myTimesVisited;
};

Math::Vector2 RotateVectorQuarterClockwise(const Math::Vector2 aDirection)
{
    return {-aDirection.myY, aDirection.myX};
}

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

Point FindGuardPoint(const Map& aMap)
{
    for (int y = 0; y < aMap.size(); ++y)
    {
        for (int x = 0; x < aMap[y].size(); ++x)
        {
            char tile = aMap[y][x];

            if (tile == '^')
            {
                Point spawnPoint;
                spawnPoint.myDirection = {0, -1};
                spawnPoint.myPosition = {x, y};

                return spawnPoint;
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
    Point guard = FindGuardPoint(aMap);

    bool isGuardOutOfBounds = false;
    while (!isGuardOutOfBounds)
    {
        const Math::Vector2 guardPos = guard.myPosition;
        aMap[guardPos.myY][guardPos.myX] = 'X';

        const Math::Vector2 aheadPos = guard.myPosition + guard.myDirection;
        if (IsPositionWithinMap(aheadPos, aMap))
        {
            const char tile = aMap[aheadPos.myY][aheadPos.myX];
            if (tile == '#')
            {
                guard.myDirection = RotateVectorQuarterClockwise(guard.myDirection);
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

int CountTraversedTiles(const Map& aMap, std::vector<Math::Vector2>& outVisitedTiles)
{
    int numExploredTiles = 0;
    for (int y = 0; y < aMap.size(); ++y)
    {
        for (int x = 0; x < aMap[y].size(); ++x)
        {
            const char tile = aMap[y][x];
            if (tile == 'X')
            {
                ++numExploredTiles;
                outVisitedTiles.push_back(Math::Vector2(x, y));
            }
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

void Part1(const std::string aPath, std::vector<Math::Vector2>& outVisitedTiles)
{
    Map map;
    ReadMap(map, aPath);

    GuardTraversesMap(map);

    // PrintMap(map);

    const int result = CountTraversedTiles(map, outVisitedTiles);

    Debug::PrintInt(result);
}

bool IsPossibleToPlaceObstruction(const Math::Vector2 aPosition, const Math::Vector2 aGuardSpawn)
{
    if (aPosition == aGuardSpawn)
    {
        return false;
    }

    return true;
}

bool IsGuardInLoop(const Point& aGuard, const std::vector<Point>& someVisitedObstacles)
{
    for (const Point& point : someVisitedObstacles)
    {
        if (point.myPosition == aGuard.myPosition && point.myDirection == aGuard.myDirection &&
            point.myTimesVisited > 1)
        {
            return true;
        }
    }

    return false;
}

int FindPossibleLoops(const Map& anOriginalMap, const std::vector<Math::Vector2>& someVisitedTiles)
{
    std::vector<Math::Vector2> loopsFound;

    Map map;
    for (const Math::Vector2 visitedTile : someVisitedTiles)
    {
        Point guard = FindGuardPoint(anOriginalMap);

        if (!IsPossibleToPlaceObstruction(visitedTile, guard.myPosition))
        {
            continue;
        }

        map.clear();
        map = anOriginalMap;
        map[visitedTile.myY][visitedTile.myX] = '0';

        std::vector<Point> visitedObstacles;
        const int maxObstaclesFound = 200;
        bool isGuardOutOfBounds = false;
        while (!isGuardOutOfBounds && visitedObstacles.size() < maxObstaclesFound)
        {
            const Math::Vector2 positionAhead = guard.myPosition + guard.myDirection;
            if (IsPositionWithinMap(positionAhead, map))
            {
                const char tile = map[positionAhead.myY][positionAhead.myX];
                if (tile == '#' || tile == '0')
                {
                    bool hasVisitedPoint = false;
                    for (Point& point : visitedObstacles)
                    {
                        if (point.myPosition == guard.myPosition && point.myDirection == guard.myDirection)
                        {
                            hasVisitedPoint = true;
                            ++point.myTimesVisited;
                            break;
                        }
                    }

                    if (!hasVisitedPoint)
                    {
                        Point turningPoint = Point(guard.myPosition, guard.myDirection);
                        visitedObstacles.push_back(turningPoint);
                    }

                    if (IsGuardInLoop(guard, visitedObstacles))
                    {
                        loopsFound.push_back(positionAhead);
                        break; // ends while loop
                    }

                    guard.myDirection = RotateVectorQuarterClockwise(guard.myDirection);
                }
                else
                {
                    guard.myPosition = positionAhead;
                }
            }
            else
            {
                isGuardOutOfBounds = true;
            }
        }
    }

    return loopsFound.size();
}

void Part2(const std::string aPath, const std::vector<Math::Vector2>& someVisitedTiles)
{
    Map map;
    ReadMap(map, aPath);

    const int result = FindPossibleLoops(map, someVisitedTiles);

    Debug::PrintInt(result);
}

int main()
{
    const std::string mapPath = "../../Inputs/puzzle_06_input.txt";
    // const std::string mapPath = "../../Inputs/puzzle_06_input_test.txt";

    std::vector<Math::Vector2> visitedTiles;
    Part1(mapPath, visitedTiles);

    Part2(mapPath, visitedTiles);

    return 0;
}
