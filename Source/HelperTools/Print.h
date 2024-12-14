#pragma once
#include <iostream>
#include <string>

namespace Debug
{
    void Print(const std::string anInput)
    {
        std::cout << anInput << std::endl;
    }

    void PrintInt(const int aValue)
    {
        std::cout << aValue << std::endl;
    }
}