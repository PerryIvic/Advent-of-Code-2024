#pragma once
#include <fstream>
#include <string>
#include "Print.h"

namespace Debug
{
    std::string LoadFile(const std::string aFileName)
    {
        std::ifstream file(aFileName);

        if (!file.is_open())
        {
            Print("failed to open " + aFileName);
        }

        std::string output;
        std::string inputLine = "";
        while (getline(file, inputLine))
        {
            output += inputLine;
        }

        return output;
    }
}
