#pragma once
#include <vector>

class Report
{
  public:
    enum class LevelType
    {
        Undefined,
        Increasing,
        Decreasing
    };

    Report(std::vector<int> levels);

    bool AreAllLevelsSafe(const bool shouldUseProblemDampener);

  private:
    LevelType GetLevelType(const int aValue);

    bool IsLevelDifferenceSafe(const int diff);

    bool IsLevelSafe(const int aDifference, const LevelType aSelectedLevelType);

    bool AreLevelsSafe(const std::vector<int>& levels);

  private:
    std::vector<int> myLevels;
};
