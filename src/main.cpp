#include <string.h>

#include <iostream>

#include "list.h"
#include "task12.h"

using namespace std;

int main(int argc, char* argv[])
{
    // FIRST
    std::cout << "Task1: " << std::endl;
    for (auto i = 0; i < 5; ++i)
    {
        auto number = (std::rand() % 9999) * (i % 2 ? -1 : 1);

        std::cout << "bitset(num = " << number << "):" << std::endl;
        std::cout << std::bitset<32>(number) << std::endl;
        std::cout << "printBinary(" << number << "):" << std::endl;
        saber_test::printBinary(number);
    }
    std::cout << "Task2: " << std::endl;
    // SECOND
    char test1[] = "AAA B CCC DEE FG HHH";
    char test2[] = "ABBCCDEEFGFG   HHH";
    saber_test::removeDups(test1);
    assert(strcmp(test1, "A B C DE FG H") == 0);
    saber_test::removeDups(test2);
    assert(strcmp(test2, "ABCDEFGFG H") == 0);
    std::cout << "Success: " << std::endl;
    // THIRD

    saber_test::workOnList();
    return 0;
}