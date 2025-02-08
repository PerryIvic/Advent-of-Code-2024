#include "Print.h"
#include <fstream>
#include <string>
#include <vector>

struct Data
{
    Data() : myId(-1), mySize(0)
    {
    }

    Data(const int anId, const int aSize) : myId(anId), mySize(aSize)
    {
    }

    int myId;
    int mySize;
};

using Disk = std::vector<int>;
using DiskData = std::vector<Data>;

void PrintDisk(const Disk& aDisk)
{
    for (size_t diskIndex = 0; diskIndex < aDisk.size(); ++diskIndex)
    {
        if (aDisk[diskIndex] == -1)
        {
            std::cout << '.';
        }
        else
        {
            std::cout << aDisk[diskIndex];
        }
    }

    std::cout << std::endl;
}

void PrintDisk(const DiskData& aDisk)
{
    for (size_t diskIndex = 0; diskIndex < aDisk.size(); ++diskIndex)
    {
        for (int times = 0; times < aDisk[diskIndex].mySize; ++times)
        {
            if (aDisk[diskIndex].myId == -1)
            {
                std::cout << '.';
            }
            else
            {
                std::cout << aDisk[diskIndex].myId;
            }
        }
    }

    std::cout << std::endl;
}

bool ReadFile(DiskData& someDiskData, const std::string aPath)
{
    std::ifstream file(aPath);

    if (!file.is_open())
    {
        Debug::Print("Failed to open " + aPath);
        return false;
    }

    int idCount = 0;
    bool isFile = true;

    std::string line;
    while (getline(file, line))
    {
        for (char cNum : line)
        {
            const int size = cNum - '0';

            int id;
            if (isFile)
            {
                id = idCount;
                ++idCount;
            }
            else
            {
                id = -1;
            }
            isFile = !isFile;

            someDiskData.push_back(Data(id, size));
        }
    }

    file.close();

    if (someDiskData.empty())
    {
        Debug::Print("aDisk is empty");
        return false;
    }

    return true;
}

bool ReadFile(Disk& aDisk, const std::string aPath)
{
    std::ifstream file(aPath);

    if (!file.is_open())
    {
        Debug::Print("Failed to open " + aPath);
        return false;
    }

    int idCount = 0;
    bool isFile = true;

    std::string line;
    while (getline(file, line))
    {
        for (char cNum : line)
        {
            const int size = cNum - '0';

            int id;
            if (isFile)
            {
                id = idCount;
                ++idCount;
            }
            else
            {
                id = -1;
            }
            isFile = !isFile;

            for (int i = 0; i < size; ++i)
            {
                aDisk.push_back(id);
            }
        }
    }

    file.close();

    if (aDisk.empty())
    {
        Debug::Print("aDisk is empty");
        return false;
    }

    return true;
}

void CompactDisk(Disk& aDisk)
{
    size_t endIndex = aDisk.size() - 1;
    for (size_t i = 0; i < aDisk.size(); ++i)
    {
        if (aDisk[i] == -1)
        {
            if (i >= endIndex)
                return;

            bool hasSwapped = false;
            while (!hasSwapped && i < endIndex)
            {
                if (aDisk[endIndex] != -1)
                {
                    std::swap(aDisk[i], aDisk[endIndex]);
                    hasSwapped = true;
                }
                --endIndex;
            }
        }
    }
}

std::pair<const Data, int> GetHighestDataIdWithinThreshold(const DiskData& someDiskData, const int aThreshold)
{
    Data highest;
    int index = 0;
    for (int i = 0; i < someDiskData.size(); ++i)
    {
        const Data data = someDiskData[i];
        if (data.myId > highest.myId && data.myId < aThreshold && data.myId != -1)
        {
            highest = data;
            index = i;
        }
    }

    return std::pair<const Data, int>(highest, index);
}

int GetLowestDataId(const DiskData& someDiskData)
{
    int lowest = INT_MAX;
    for (const Data data : someDiskData)
    {
        if (data.myId < lowest && data.myId != -1)
        {
            lowest = data.myId;
        }
    }

    return lowest;
}

void CompactDisk(DiskData& someDiskData)
{
    const int lowestId = GetLowestDataId(someDiskData);
    int highestIDThreshold = INT_MAX;

    bool hasCompactedAllFiles = false;
    while (!hasCompactedAllFiles)
    {
        if (highestIDThreshold == lowestId)
        {
            hasCompactedAllFiles = true;
        }

        const auto [prioritizedData, highestIndex] = GetHighestDataIdWithinThreshold(someDiskData, highestIDThreshold);
        highestIDThreshold = prioritizedData.myId;

        for (int dataIndex = 0; dataIndex < highestIndex; ++dataIndex)
        {
            Data& emptyData = someDiskData[dataIndex];
            if (emptyData.myId == -1 && emptyData.mySize >= prioritizedData.mySize)
            {
                if (emptyData.mySize > prioritizedData.mySize)
                {
                    const int remainingSpace = emptyData.mySize - prioritizedData.mySize;
                    emptyData.mySize = prioritizedData.mySize;

                    std::swap(someDiskData[dataIndex], someDiskData[highestIndex]);
                    someDiskData.insert(someDiskData.begin() + dataIndex + 1, Data(-1, remainingSpace));
                }
                else
                {
                    std::swap(someDiskData[dataIndex], someDiskData[highestIndex]);
                }

                break;
            }
        }
    }
}

int64_t GetCheckSum(const Disk& aDisk)
{
    int64_t checkSum = 0;
    for (size_t i = 0; i < aDisk.size(); ++i)
    {
        if (aDisk[i] == -1)
            break;

        checkSum += i * aDisk[i];
    }

    return checkSum;
}

int64_t GetCheckSum(const DiskData& aDisk)
{
    int64_t checkSum = 0;

    size_t indexPos = 0;
    for (size_t index = 0; index < aDisk.size(); ++index)
    {
        const Data data = aDisk[index];
        if (data.myId == -1)
        {
            indexPos += data.mySize;
            continue;
        }

        for (size_t count = 0; count < data.mySize; ++count)
        {
            checkSum += indexPos * data.myId;
            ++indexPos;
        }
    }

    return checkSum;
}

int64_t Part1(const std::string aFilePath)
{
    Disk disk;
    if (!ReadFile(disk, aFilePath))
        return -1;

    CompactDisk(disk);

    // PrintDisk(disk);

    return GetCheckSum(disk);
}

int64_t Part2(const std::string aFilePath)
{
    DiskData diskData;
    if (!ReadFile(diskData, aFilePath))
        return -1;

    CompactDisk(diskData);

    // PrintDisk(diskData);

    return GetCheckSum(diskData);
}

int main()
{
    const std::string filePath = "../../Inputs/puzzle_09_input.txt";
    // const std::string filePath = "../../Inputs/puzzle_09_test_input.txt";

    const int64_t resultPart1 = Part1(filePath);
    std::cout << resultPart1 << std::endl;

    const int64_t resultPart2 = Part2(filePath);
    std::cout << resultPart2 << std::endl;

    return 0;
}
