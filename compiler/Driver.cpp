#include "Driver.hpp"
#include "Error.hpp"

#include <fstream>
#include <cstdio>
#include <cstdarg>
#include <algorithm>

namespace
{
    
}


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
    const auto& name = function.name;
    const auto  defined = (function.body != nullptr);
    
    context.add_to_symbol_table(ti::Symbol
    {
        .type = SymbolType::FUNCTION,
        .name = name,
        .defined = defined,
    });

    context.add_to_code(ti::format("@function_start_%s:\n", function.name.c_str()));
    
    //if function is defined
    if (defined)
    {
        function.body->generate(context, function);
    }
    
    context.add_to_code(ti::format("@function_end_%s:\n", name.c_str()));
    
    //issue function return
    context.add_to_code("pop IP");
}

void ti::generate_statement(ti::Context& context, ti::Function& function, ti::Statement* statement) noexcept
{
    statement->generate(context, function);
}

//reformatted from https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template<typename... Args>
std::string ti::format(const std::string& format, Args&&... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    
    if (size_s <= 0 )
    {
        throw std::runtime_error("Error during formatting.");
    }
    
    auto size = static_cast<size_t>(size_s);
    
    std::unique_ptr<char[]> buf(new char[size]);
    
    std::snprintf(buf.get(), size, format.c_str(), args...);
    
    return std::string( buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
