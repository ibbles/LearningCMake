#include "config.h"

#include <iostream>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout
            << argv[0] << " Version "
            << exFallingBody_VERSION_MAJOR << '.' << exFallingBody_VERSION_MINOR
            << '\n';
        #if MY_VERBOSE
        std::cout
            << "Major version is " << exFallingBody_VERSION_MAJOR << '\n'
            << "Minor version is " << exFallingBody_VERSION_MINOR << '\n';
        #endif
        return 1;
    }

    double value = std::stod(argv[1]);
    std::cout << "The value is "<< value << '\n';
    return 0;
}
