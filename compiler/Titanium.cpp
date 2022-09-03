#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Driver.hpp"

int main(int argc, char** argv)
{
    int res = 0;
    
    driver drv;
    
    if (argc == 2)
    {
        if (!drv.parse(argv[1]))
        {
            std::cout << drv.result << '\n';
        }
    }
    
    
    
    return res;
}
