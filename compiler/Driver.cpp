#include "Driver.hpp"
#include "Error.hpp"

#include <fstream>
#include <format>
#include <algorithm>

void ti::generate_program(ti::Program& program, ti::Parameters& parameters) noexcept
{
    auto context = ti::Context{};
    
    context.add_to_code(".begin\n");
    context.add_to_code(".include \"def.s\"\n");
    
    if (std::find_if(program.functions.begin(), program.functions.end(), [&](Function f)
        {
            return f.name == "main";
        }) == std::end(program.functions))
    {
        //no main function
        ti::throw_error("No 'main' function was defined");
    }
    
    
    std::for_each(program.functions.begin(), program.functions.end(), [&](Function& f)
    {
        ti::generate_function(context, f);
    });
    
    context.add_to_code(".end\n");
    
    const auto path = parameters.file_name + ".s";
    std::ofstream file(path);
    
    if (!file.good())
    {
        ti::throw_error("Could not open file %s for writing", path.c_str());
    }
    
    file << context.code_segment;
}


void ti::generate_function(ti::Context& context, ti::Function& function) noexcept
{
    context.add_to_symbol_table(ti::Symbol
    {
        .type = SymbolType::FUNCTION,
        .name = function.name,
        .defined = (function.body != nullptr),
    });
    
    context.add_to_code(std::format("@function_start_{}:", function.name));
    
    
}
