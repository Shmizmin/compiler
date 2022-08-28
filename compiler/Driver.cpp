#include "Driver.hpp"
#include "Error.hpp"

#include <fstream>
#include <cstdio>
#include <cstdarg>
#include <algorithm>
#include <iostream>


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
    
    context.add_to_code(context.end_segment);
    
    context.add_to_code(".end\n");
    
    const auto path = parameters.file_name + ".s";
    
    auto file = std::fopen(path.c_str(), "w");
    
    std::fwrite(context.code_segment.data(), sizeof(context.code_segment[0]), context.code_segment.size(), file);
}


void ti::generate_function(ti::Context& context, ti::Function& function) noexcept
{
    const auto& name = function.name;
    const auto  defined = (function.body != nullptr);
    
    const auto label = ti::format("function_start_%s", function.name.c_str());
    
    context.add_to_symbol_table(new ti::FunctionSymbol
    {
        SymbolType::FUNCTION,
        name,
        defined,
        {}
    });

    context.add_to_code(ti::format("@%s:\n", label.c_str()));
    
    //if function is defined
    if (defined)
    {
        function.body->generate(context, function);
    }
    
    context.add_to_code(ti::format("@function_end_%s:\n", name.c_str()));
    
    //issue function return
    context.add_to_code("\tpop IP\n");
}

void ti::generate_statement(ti::Context& context, ti::Function& function, ti::Statement* statement) noexcept
{
    statement->generate(context, function);
}
