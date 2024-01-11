// main: c_basic - Simple BASIC interpreter

#include <iostream>
#include "parser.h"

int main(int argc, char *argv[])
{
    // Check if the filename is provided as an argument
    if (argc != 2)
    {
        std::cerr << "Usage: c_basic filename\n";
        return 1;
    }

    // Create a Parser object with the filename as an argument
    Parser parser(argv[1]);

    // Print a message to indicate the start of parsing
    std::cout << "c_basic: " << argv[1] << std::endl;

    // Parse the file
    parser.parse();

    return 0;
}