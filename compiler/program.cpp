#include "program.hpp"
#include "compiler.hpp"
#include "function.hpp"
#include "error.hpp"

#include <fstream>
#include <fmt/format.h>

namespace
{
    void optimize_ast(ti::Compiler& compiler) noexcept
    {
        
    }
    
    void optimize_ir(ti::Compiler& compiler) noexcept
    {
        /*tail jump elimination
        {
            std::regex rx(R"(\tjmp\(function_end_([a-zA-Z][a-zA-Z_0-9]*)\)\n@function_end_\1:)");
            std::smatch matches;
            
            while (std::regex_search(code, matches, rx))
            {
                ::replace(code, matches[0].str(), fmt::format("@function_end_{}:", matches[1].str()));
            }
        */
        
        // peephole optimizations
    }
}


namespace ti
{
    void compile_program(Program* program, Compiler& compiler, Parameters& parameters) noexcept
    {
        {
            bool found = false;
            for (auto function : program->functions)
            {
                compile_function(function, compiler);
                
                if (function->name == "main") [[unlikely]]
                {
                    found = true;
                }
            }
            
            if (!found)
            {
                throw_error("No \'main\' function was defined");
            }
        }
        
        
        if (parameters.optimize_ast)
        {
            ::optimize_ast(compiler);
        }
            
        if (parameters.optimize_ir)
        {
            ::optimize_ir(compiler);
        }
            
        
        const auto path = (parameters.file_name + ".s");
        if (auto file = std::ofstream(path); file.good())
        {
            // TODO: maybe move return value? unsure of copy elision rules
            const auto data = generate_commands(compiler.ir_code);
            
            file.write(data.data(), data.size());
        }
        
        else
        {
            throw_error(fmt::format("File {} could not be opened for writing", path));
        }
    }
}
