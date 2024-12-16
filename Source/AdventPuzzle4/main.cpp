#include <fstream>
#include <functional>
#include <vector>
#include <array>
#include <algorithm>

#include "CoordinateIterator.h"
#include "Print.h"

using Grid = std::vector<std::vector<char>>;
using Pattern = std::function<int(const std::string, const Grid &)>;

void ReadFilePart1(Grid& aGrid, const std::string aFilePath)
{
    std::ifstream file(aFilePath);

    if (!file.is_open())
    {
        Debug::Print("failed to open " + aFilePath);
    }

    std::string inputLine = "";
    while (getline(file, inputLine))
    {
        std::vector<char> row(inputLine.begin(), inputLine.end());
        aGrid.push_back(row);
    }

    file.close();

    if (aGrid.empty())
    {
        Debug::Print("Grid size is empty!");
    }
}

void ReadFilePart2(Grid& aGrid, const std::string aFilePath)
{
    std::ifstream file(aFilePath);

    if (!file.is_open())
    {
        Debug::Print("failed to open " + aFilePath);
    }

    std::string inputLine = "";
    while (getline(file, inputLine))
    {
        std::vector<char> row(inputLine.begin(), inputLine.end());
        std::reverse(row.begin(), row.end());

        aGrid.push_back(row);
    }

    file.close();

    if (aGrid.empty())
    {
        Debug::Print("Grid size is empty!");
    }
}

std::string GetWordReversed(const std::string aWord)
{
    std::string wordReversed = aWord;
    std::reverse(wordReversed.begin(), wordReversed.end());

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
            CoordinateIterator iterationData = CoordinateIterator(
                x, CoordinateIterator::IterationMethod::Add,
                y, CoordinateIterator::IterationMethod::None);

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
            CoordinateIterator iterationData = CoordinateIterator(
                x, CoordinateIterator::IterationMethod::None, 
                y, CoordinateIterator::IterationMethod::Add);

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

            std::vector<CoordinateIterator> coordIterations;
            coordIterations.push_back(southEastDirection);
            coordIterations.push_back(southWestDirection);
            coordIterations.push_back(northEastDirection);
            coordIterations.push_back(northWestDirection);

            for (CoordinateIterator it : coordIterations)
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

int Part1()
{
    //std::string filePath = "../../Inputs/puzzle_04_test_input.txt"; // temp
    std::string filePath = "../../Inputs/puzzle_04_input.txt";
    
    Grid grid;
    ReadFilePart1(grid, filePath);

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
        result += pattern(targetWord, grid);
    }

    return result;
}

int XmasSearch(const Grid& aGrid)
{
    int numTotalMatchesFound = 0;

    const int wordSize = 3;
    const std::array<std::string, 2> words = { "MAS" ,"SAM" };

    const int height = aGrid.size() - wordSize;
    for (int northWordBound = 0; northWordBound <= height; ++northWordBound)
    {
        const int width = aGrid[northWordBound].size() - wordSize;
        for (int x = 0; x <= width; ++x)
        {
            const int southWordBound = northWordBound + wordSize -1;

            const CoordinateIterator directionSE = CoordinateIterator(
                x, CoordinateIterator::IterationMethod::Add,
                northWordBound, CoordinateIterator::IterationMethod::Add);

            const CoordinateIterator directionNE = CoordinateIterator(
                x, CoordinateIterator::IterationMethod::Add,
                southWordBound, CoordinateIterator::IterationMethod::Subtract);

            const std::array<CoordinateIterator, 2> iterators = { directionSE, directionNE };

            int numMatches = 0;
            for (const CoordinateIterator it : iterators)
            {
                for (const std::string word : words)
                {
                    if (ContainsTargetWord(aGrid, word, it))
                    {
                        ++numMatches;
                    }
                }
            }

            if (numMatches == 2)
            {
                ++numTotalMatchesFound;
            }
        }
    }

    return numTotalMatchesFound;
}

int Part2()
{
    std::string filePath = "../../Inputs/puzzle_04_input.txt";
    Grid grid;
    ReadFilePart2(grid, filePath);

    int numFound = XmasSearch(grid);

    return numFound;
}

int main()
{
    const int resultsPart1 = Part1();
    const int resultPart2 = Part2();

    Debug::PrintInt(resultsPart1);
    Debug::PrintInt(resultPart2);

    return 0;
}
