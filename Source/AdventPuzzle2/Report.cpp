#include "Report.h"
#include <cmath>

Report::Report(std::vector<int> levels) : myLevels(levels)
{
}

Report::LevelType Report::GetLevelType(const int aValue)
{
    if (aValue == 0)
        return LevelType::Undefined;

    bool isDecreasing = std::signbit(static_cast<float>(aValue));
    return isDecreasing ? LevelType::Decreasing : LevelType::Increasing;
}

bool Report::IsLevelDifferenceSafe(const int diff)
{
    return diff >= 1 && diff <= 3;
}

bool Report::IsLevelSafe(const int aDifference, const LevelType aSelectedLevelType)
{
    LevelType levelType = GetLevelType(aDifference);

    if (levelType == LevelType::Undefined || levelType != aSelectedLevelType ||
        !IsLevelDifferenceSafe(abs(aDifference)))
    {
        return false;
    }

    return true;
}

bool Report::AreLevelsSafe(const std::vector<int> &levels)
{
    int previousDigit = -1;
    LevelType levelType = LevelType::Undefined;

    for (int i = 0; i < levels.size(); ++i)
    {
        const char digit = levels[i];
        if (previousDigit != -1)
        {
            const int difference = digit - previousDigit;
            LevelType method = GetLevelType(difference);

            if (levelType == LevelType::Undefined)
            {
                levelType = method;
            }

            if (!IsLevelSafe(difference, levelType))
            {
                return false;
            }
        }
        previousDigit = digit;
    }

    return true;
}

bool Report::AreAllLevelsSafe(const bool shouldUseProblemDampener)
{
    if (shouldUseProblemDampener)
    {
        if (!AreLevelsSafe(myLevels))
        {
            // Part 2 Start
            std::vector<int> dampenedLevels;
            int iterations = 0;
            while (iterations < myLevels.size())
            {
                dampenedLevels = myLevels;
                dampenedLevels.erase(dampenedLevels.begin() + iterations);
                if (AreLevelsSafe(dampenedLevels))
                {
                    return true;
                }

                dampenedLevels.clear();
                ++iterations;
            }
            // Part 2 End

            return false;
        }

        return true;
    }
    else
    {
        return AreLevelsSafe(myLevels);
    }
}
