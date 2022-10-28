#include "Central.hpp"
#include "Error.hpp"
#include "Bytecode.hpp"
#include "Allocation.hpp"

#include <fstream>
#include <cstdio>
#include <cstdarg>
#include <iostream>
#include <regex>
#include <fmt/format.h>

namespace
{
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
        for (auto& function : program.functions)
        {
            compile_function(context, function);
            
            if (function.name == "main")
                found = true;
        }
        
        if (!found)
            throw_error("No \'main\' function was defined");
        
        
        if (parameters.optimize_ast)
            ::optimize_ast(context);
        
        if (parameters.optimize_ir)
            ::optimize_ir(context);
        
        
        const auto path = (parameters.file_name + ".s");
        if (auto file = std::ofstream(path); file.good())
        {
            //maybe move return value? unsure of copy elision rules
            const auto data = generate_commands(context.ir_code);
            
            file.write(data.data(), data.size());
        }
        
        else
        {
            throw_error(fmt::format("File {} could not be opened for writing", path));
        }
    }
    
    void compile_function(Context& context, Function& function) noexcept
    {
        const auto& name    =  function.name;
        const auto  defined = (function.body != nullptr);
        
        const auto label = fmt::format("function_start_{}", name);
        
        context.add_to_symbol_table(new ti::Symbol
        {
            .type = SymbolType::FUNCTION,
            .name = name,
            .defined = defined,
            .as.function.arguments = function.arguments,
        });

        context.emit_label(fmt::format("@{}:\n", label));
        
        //generate code to create local variables for each function parameter
        {
            auto* var_stmt = new Statement
            {
                .type = ti::StatementType::VARIABLE,
                .as.variable.variables = {},
            };
            
            for (auto i = 0; i < function.arguments.size(); ++i)
            {
                const auto& arg = function.arguments[i];
                
                //will leak
                auto* var = new ti::Variable();

                var->name = fmt::format("{}_{}", name, arg.name);
                var->type = arg.type;
                var->visibility = ti::TypeVisibility::LOCAL;
                var->value = nullptr;

                //var_stmt.
                
                //var_stmt->variables.emplace_back(var);
            
            }
            
            ti::RegisterAllocation new_allocation{ context };
            ti::CommonArgs args{ context, function, new_allocation.location };
            ti::compile_statement(var_stmt, args);
        }
        
        //if function is defined
        if (defined)
        {
            ti::RegisterAllocation new_allocation{ context };
            ti::CommonArgs args{ context, function, new_allocation.location };
            ti::compile_statement(function.body, args);
        }
        
        context.emit_label(fmt::format("@function_end_{}:\n", name));
        
        //issue functionreturn
        context.emit_pop(RegisterType::IP);
    }

}
