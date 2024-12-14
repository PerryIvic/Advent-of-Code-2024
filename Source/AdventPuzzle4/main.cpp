#include <fstream>
#include <functional>
#include <vector>

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

bool HasBuiltTargetWord(const char aLetter, std::string &someOutWordsFound, const std::string aTargetWord)
{
    const int wordIndex = someOutWordsFound.size();
    if (aLetter == aTargetWord[wordIndex])
    {
        someOutWordsFound += aLetter;
    }
    else
    {
        someOutWordsFound = "";
    }

    return someOutWordsFound == aTargetWord;
}

int HorizontalSearch(const std::string aWord, const Grid &aGrid)
{
    int num = 0;

    for (int y = 0; y < aGrid.size(); ++y)
    {
        const int width = aGrid[y].size() - aWord.size();
        for (int x = 0; x < width; ++x)
        {
            std::string wordsFound;
            for (int it = 0; it < aWord.size(); ++it)
            {
                const char letter = aGrid[y][x + it];
                if (letter != aWord[wordsFound.size()])
                {
                    break;
                }

                if (HasBuiltTargetWord(letter, wordsFound, aWord))
                {
                    wordsFound = "";
                    ++num;
                }
            }

            wordsFound = "";
        }
    }

    return num;
}

int HorizontalReversedSearch(const std::string aWord, const Grid &aGrid)
{
    return HorizontalSearch(GetWordReversed(aWord), aGrid);
}

int VerticalSearch(const std::string aWord, const Grid &aGrid)
{
    int num = 0;

    for (int x = 0; x < aGrid.size(); ++x)
    {
        const int height = aGrid.size() - aWord.size();
        for (int y = 0; y <= height; ++y)
        {
            std::string wordsFound;
            for (int it = 0; it < aWord.size(); ++it)
            {
                const char letter = aGrid[y + it][x];
                if (letter != aWord[wordsFound.size()])
                {
                    break;
                }

                if (HasBuiltTargetWord(letter, wordsFound, aWord))
                {
                    wordsFound = "";
                    ++num;
                }
            }
        }
    }

    return num;
}

int VerticalReversedSearch(const std::string aWord, const Grid &aGrid)
{
    return VerticalSearch(GetWordReversed(aWord), aGrid);
}

int DiagonalSearch(const std::string aWord, const Grid &aGrid)
{
    int num = 0;

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

            std::string wordsFoundSE;
            for (int it = 0; it < aWord.size(); ++it) // South-East
            {
                const char letter = aGrid[northBoundY + it][westBoundX + it];
                if (letter != aWord[wordsFoundSE.size()])
                {
                    break;
                }

                if (HasBuiltTargetWord(letter, wordsFoundSE, aWord))
                {
                    wordsFoundSE = "";
                    ++num;
                }
            }

            std::string wordsFoundSW;
            for (int it = 0; it < aWord.size(); ++it) // South-West
            {
                const char letter = aGrid[northBoundY + it][eastBoundX - it];
                if (letter != aWord[wordsFoundSW.size()])
                {
                    break;
                }

                if (HasBuiltTargetWord(letter, wordsFoundSW, aWord))
                {
                    wordsFoundSW = "";
                    ++num;
                }
            }

            std::string wordsFoundNE;
            for (int it = 0; it < aWord.size(); ++it) // North-East
            {
                const char letter = aGrid[southBoundY - it][westBoundX + it];
                if (letter != aWord[wordsFoundNE.size()])
                {
                    break;
                }

                if (HasBuiltTargetWord(letter, wordsFoundNE, aWord))
                {
                    wordsFoundNE = "";
                    ++num;
                }
            }

            std::string wordsFoundNW;
            for (int it = 0; it < aWord.size(); ++it) // North-West
            {
                const char letter = aGrid[southBoundY - it][eastBoundX - it];
                if (letter != aWord[wordsFoundNW.size()])
                {
                    break;
                }

                if (HasBuiltTargetWord(letter, wordsFoundNW, aWord))
                {
                    wordsFoundNW = "";
                    ++num;
                }
            }
        }
    }

    return num;
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
    std::string fileName = "../../Inputs/puzzle_04_test_input.txt"; // temp
    // std::string fileName = "../../Inputs/puzzle_04_input.txt";
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
