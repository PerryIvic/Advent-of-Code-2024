#include <fstream>
#include <functional>
#include <vector>

#include "CoordinateIterator.h"
#include "Print.h"

using Grid = std::vector<std::vector<char>>;
using Pattern = std::function<int(const std::string, const Grid &)>;

std::string GetWordReversed(const std::string aWord)
{
    std::string wordReversed;
    for (int i = aWord.size() - 1; i >= 0; --i)
    {
        wordReversed += aWord[i];
    }

    return wordReversed;
}

bool ContainsTargetWord(const Grid &aGrid,
    const std::string aTargetWord,
    const CoordinateIterator aCoordIterationData)
{
    std::string lettersFound;
    for (int it = 0; it < aTargetWord.size(); ++it)
    {
        const int x = aCoordIterationData.GetOffsettedX(it);
        const int y = aCoordIterationData.GetOffsettedY(it);

        const char letter = aGrid[y][x];
        if (letter == aTargetWord[lettersFound.size()])
        {
            lettersFound += letter;
        }
        else
        {
            return false;
        }

        if (lettersFound == aTargetWord)
        {
            return true;
        }
    }

    return false;
}

int HorizontalSearch(const std::string aWord, const Grid &aGrid)
{
    int numWordsFound = 0;

    for (int y = 0; y < aGrid.size(); ++y)
    {
        const int width = aGrid[y].size() - aWord.size();
        for (int x = 0; x <= width; ++x)
        {
            CoordinateIterator iterationData = CoordinateIterator(x, CoordinateIterator::IterationMethod::Add, y,
                                                                  CoordinateIterator::IterationMethod::None);

            if (ContainsTargetWord(aGrid, aWord, iterationData))
            {
                ++numWordsFound;
            }
        }
    }

    return numWordsFound;
}

int HorizontalReversedSearch(const std::string aWord, const Grid &aGrid)
{
    return HorizontalSearch(GetWordReversed(aWord), aGrid);
}

int VerticalSearch(const std::string aWord, const Grid &aGrid)
{
    int numWordsFound = 0;

    for (int x = 0; x < aGrid.size(); ++x)
    {
        const int height = aGrid.size() - aWord.size();
        for (int y = 0; y <= height; ++y)
        {
            CoordinateIterator iterationData = CoordinateIterator(x, CoordinateIterator::IterationMethod::None, y,
                                                                  CoordinateIterator::IterationMethod::Add);

            if (ContainsTargetWord(aGrid, aWord, iterationData))
            {
                ++numWordsFound;
            }
        }
    }

    return numWordsFound;
}

int VerticalReversedSearch(const std::string aWord, const Grid &aGrid)
{
    return VerticalSearch(GetWordReversed(aWord), aGrid);
}

int DiagonalSearch(const std::string aWord, const Grid &aGrid)
{
    int numWordsFound = 0;

    const int height = aGrid.size() - aWord.size();
    if (height < 0)
    {
        Debug::Print("Grid size to small!");
        return 0;
    }

    for (int northBoundY = 0; northBoundY <= height; ++northBoundY)
    {
        const int width = aGrid[northBoundY].size() - aWord.size();
        if (width < 0)
        {
            continue;
        }

        const int southBoundY = (aGrid.size() - 1) - northBoundY;
        for (int westBoundX = 0; westBoundX <= width; ++westBoundX)
        {
            const int eastBoundX = (aGrid[westBoundX].size() - 1) - westBoundX;

            CoordinateIterator southEastDirection = CoordinateIterator(
                westBoundX, CoordinateIterator::IterationMethod::Add,
                northBoundY, CoordinateIterator::IterationMethod::Add);

            CoordinateIterator southWestDirection = CoordinateIterator(
                eastBoundX, CoordinateIterator::IterationMethod::Subtract, 
                northBoundY, CoordinateIterator::IterationMethod::Add);

            CoordinateIterator northEastDirection = CoordinateIterator(
                westBoundX, CoordinateIterator::IterationMethod::Add,
                southBoundY, CoordinateIterator::IterationMethod::Subtract);

            CoordinateIterator northWestDirection = CoordinateIterator(
                eastBoundX, CoordinateIterator::IterationMethod::Subtract,
                southBoundY, CoordinateIterator::IterationMethod::Subtract);

            std::vector<CoordinateIterator> iterators;
            iterators.push_back(southEastDirection);
            iterators.push_back(southWestDirection);
            iterators.push_back(northEastDirection);
            iterators.push_back(northWestDirection);

            for (CoordinateIterator it : iterators)
            {
                if (ContainsTargetWord(aGrid, aWord, it))
                {
                    ++numWordsFound;
                }
            }
        }
    }

    return numWordsFound;
}

int Part1(const Grid &aGrid)
{
    const std::string targetWord = "XMAS";

    std::vector<Pattern> searchPatterns;

    searchPatterns.push_back(HorizontalSearch);
    searchPatterns.push_back(HorizontalReversedSearch);
    searchPatterns.push_back(VerticalSearch);
    searchPatterns.push_back(VerticalReversedSearch);
    searchPatterns.push_back(DiagonalSearch);

    int result = 0;
    for (Pattern pattern : searchPatterns)
    {
        result += pattern(targetWord, aGrid);
    }

    return result;
}

int main()
{
    //std::string fileName = "../../Inputs/puzzle_04_test_input.txt"; // temp
    std::string fileName = "../../Inputs/puzzle_04_input.txt";
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        Debug::Print("failed to open " + fileName);
    }

    Grid grid;
    std::string inputLine = "";
    while (getline(file, inputLine))
    {
        std::vector<char> row(inputLine.begin(), inputLine.end());
        grid.push_back(row);
    }

    if (grid.empty())
    {
        Debug::Print("Grid size is empty!");
        return 0;
    }

    const int result = Part1(grid);
    Debug::PrintInt(result);

    return 0;
}
