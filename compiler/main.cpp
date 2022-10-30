#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <fmt/format.h>


#include "driver.hpp"
#include "error.hpp"
#include "preprocessor.hpp"

int main(int argc, char** argv)
{
    if (argc == 2)
    {
        std::string in_path = argv[1];
        const auto size = std::filesystem::file_size(in_path); //bytes
        std::ifstream in_file(in_path, std::ios::in);
        
        if (!in_file.good())
        {
            ti::throw_error(fmt::format("Could not open file {} for reading", in_path));
        }
        
        std::string buffer(size, '\0');
        in_file.read(buffer.data(), size);
        
        ti::preprocess(buffer, in_path);
        
        const auto out_path = in_path + ".i";
        std::ofstream out_file(out_path, std::ios::out);
        
        if (!out_file.good())
        {
            ti::throw_error(fmt::format("Could not open file {} for writing", out_path));
        }
        
        out_file.write(buffer.c_str(), buffer.length());
        out_file.flush();
        
        driver drv{};
        
        if (!drv.parse(out_path))
        {
            std::cout << drv.result << '\n';
        }

        return EXIT_SUCCESS;
    }
   
    else
    {
        ti::throw_error(fmt::format("Expected {} command line arguments but {} suppplied", 2, argc));
    }
}
