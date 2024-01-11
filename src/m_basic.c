// main: c_basic - Simple BASIC interpreter

#include <iostream>
#include "parser.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: c_basic filename\n";
        return 1;
    }

    std::cout << std::endl
              << "c_basic:" << std::endl;

    Parser P(argv[1]);
    P.parse();

    return 0;
}
