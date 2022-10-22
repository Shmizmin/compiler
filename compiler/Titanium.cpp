#include <iostream>
#include <fstream>
#include <string>


#include <cstdio>
#include <cassert>
#include <cstdlib>

#include "Driver.hpp"
#include "Preprocessor.hpp"

int main(int argc, char** argv)
{
    assert(argc == 2);

    std::string filepath = argv[1];
    const auto size = std::filesystem::file_size(filepath); //bytes
    std::ifstream file(filepath, std::ios::in);
    
    if (!file.good())
    {
        std::fprintf(stderr, "[Error] Could not open file %s for reading\nExiting...\n", filepath.c_str());
        return EXIT_FAILURE;
    }
    
    std::string buffer(size, '\0');
    file.read(buffer.data(), size);
    
    ti::preprocess(buffer, filepath);
    
    const auto new_path = filepath + ".i";
    std::ofstream out(new_path);
    
    if (!out.good())
    {
        std::fprintf(stderr, "[Error] Could not open file %s for writing\nExiting...\n", new_path.c_str());
        return EXIT_FAILURE;
    }
    
    out.write(buffer.c_str(), buffer.length());
    out.flush();
    
    Driver drv{};
    
    if (!drv.parse(new_path))
    {
        std::cout << drv.result << '\n';
    }

    return EXIT_SUCCESS;
}
