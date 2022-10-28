#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <fmt/format.h>


#include "Driver.hpp"
#include "Preprocessor.hpp"

int main(int argc, char** argv)
{
    if (argc == 2)
    {
        std::string filepath = argv[1];
        const auto size = std::filesystem::file_size(filepath); //bytes
        std::ifstream file(filepath, std::ios::in);
        
        if (!file.good())
        {
            std::cerr << fmt::format("[Error] Could not open file {} for reading\nExiting...\n", filepath);
            return EXIT_FAILURE;
        }
        
        std::string buffer(size, '\0');
        file.read(buffer.data(), size);
        
        ti::preprocess(buffer, filepath);
        
        const auto new_path = filepath + ".i";
        std::ofstream out(new_path);
        
        if (!out.good())
        {
            std::cerr << fmt::format("[Error] Could not open file {} for writing\nExiting...\n", new_path);
            return EXIT_FAILURE;
        }
        
        out.write(buffer.c_str(), buffer.length());
        out.flush();
        
        driver drv{};
        
        if (!drv.parse(new_path))
        {
            std::cout << drv.result << '\n';
        }

        return EXIT_SUCCESS;
    }
   
    else
    {
        std::cerr << fmt::format("[Error] Incorrect number of command line arguments supplied");
        return EXIT_FAILURE;
    }
}
