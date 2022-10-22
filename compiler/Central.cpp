#include "Central.hpp"
#include "Error.hpp"
#include "fmt/format.h"
#include "IR.hpp"

#include <fstream>
#include <cstdio>
#include <cstdarg>
#include <algorithm>
#include <iostream>
#include <regex>

namespace
{
    void replace(std::string& str, const std::string& from, const std::string& to) noexcept
    {
        if(from.empty()) return;
        std::size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }
    
    void optimize_ast(ti::Context& context) noexcept
    {
        
    }
    
    void optimize_ir(ti::Context& context) noexcept
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
    void compile_program(Program& program, Parameters& parameters) noexcept
    {
        auto context = ti::Context{};
        
        bool found = false;
        for (auto function : program.functions)
        {
            compile_function(context, function);
            
            if (function->name == "main")
                found = true;
        }
        
        if (!found)
            throw_error("No \'main\' function was defined");
        
        
        if (parameters.optimize_ast)
            ::optimize_ast(context);
        
        if (parameters.optimize_ir)
            ::optimize_ir(context);
        
        
        if (auto file = std::ofstream(parameters.file_name + ".s"); file.good())
        {
            //const auto data = std::move(generate_
            const auto data = generate_commands(context.ir_code);
            
            
            /*
             const auto path = parameters.file_name + ".s";
             
             auto file = std::fopen(path.c_str(), "w");
             
             std::fwrite(context.code_segment.data(), sizeof(context.code_segment[0]), context.code_segment.size(), file);
             
             std::fclose(file);
             */
            //file.write(context.)
        }
        
        // TODO: throw error if no file
    }
    
    void compile_function(Context& context, Function* function) noexcept
    {
        const auto& name    =  function->name;
        const auto  defined = (function->body != nullptr);
        
        const auto label = fmt::format("function_start_{}", name);
        
        context.add_to_symbol_table(new ti::Symbol
        {
            .type = SymbolType::FUNCTION,
            .name = name,
            .defined = defined,
            .as.function.arguments = function->arguments,
        });

        context.emit_label(fmt::format("@{}:\n", label));
        
        //generate code to create local variables for each function parameter
        {
            auto* var_stmt = new Statement
            {
                .type = ti::StatementType::VARIABLE,
                .as.variable.variables = {},
            };
            
            for (auto i = 0; i < function->arguments.size(); ++i)
            {
                const auto& arg = function->arguments[i];
                
                //will leak
                auto* var = new ti::Variable();

                var->name = fmt::format("{}_{}", name, arg.name);
                var->type = arg.type;
                var->visibility = ti::TypeVisibility::LOCAL;
                var->value = nullptr;

                var_stmt->variables.emplace_back(var);
            
            }
            
            ti::ForcedRegisterAllocation new_allocation{ context };
            ti::compile_statement(var_stmt, { context, function, new_allocation });
        }
        
        //if function is defined
        if (defined)
        {
            ti::ForcedRegisterAllocation new_allocation{ context };
            ti::CommonArgs args{ context, *function, new_allocation.location };
            ti::compile_statement(function->body, args);
        }
        
        context.emit_label(fmt::format("@function_end_{}:\n", name));
        
        //issue functionreturn
        context.emit_pop(RegisterType::IP);
    }

}
