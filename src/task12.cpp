#include "task12.h"

#include <iostream>

namespace saber_test
{
void printBinary(int number)
{
    for (int i = sizeof(number) * 8 - 1; i >= 0; --i)
        std::cout << ((number >> i) & 1);

    std::cout << std::endl;
}
void removeDups(char* str)
{
    if (*str == '\0')
        return;

    for (char* second = str + 1; *second != '\0'; ++second)
    {
        if (*str != *second)
            *++str = *second;
    }
    *++str = '\0';
}
}; // namespace saber_test