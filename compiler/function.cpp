#include "function.hpp"
#include "compiler.hpp"
#include "statement.hpp"
#include "allocation.hpp"

#include <fmt/format.h>

namespace
{
    
}


namespace ti
{
    void compile_function(Function* function, Compiler& compiler) noexcept
    {
        compiling_function = function;
        
        const auto& name    =  function->name;
        const auto  defined = (function->body != nullptr);
        
        const auto label = fmt::format("function_start_{}", name);
        
        compiler.add_to_symbol_table(new ti::Symbol
        {
            .type = SymbolType::FUNCTION,
            .name = name,
            .defined = defined,
            .as.function.arguments = function->arguments,
        });

        compiler.emit_label(fmt::format("@{}:\n", label));
        
        //generate code to create local variables for each function parameter
        {
            auto* variable_statement = new Statement
            {
                .type = ti::StatementType::VARIABLE,
                .as.variable.variables = {},
            };
            
            for (auto i = 0; i < function->arguments.size(); ++i)
            {
                const auto& argument = function->arguments[i];
                
                //will leak
                auto* variable = new ti::Variable();

                variable->name = fmt::format("{}_{}", name, argument.name);
                variable->type = argument.type;
                variable->visibility = ti::TypeVisibility::LOCAL;
                variable->value = nullptr;

                //var_stmt.
                
                //var_stmt->variables.emplace_back(var);
            
            }
            
            ti::compile_statement(variable_statement, compiler);
        }
        
        //if function is defined
        if (defined)
        {
            ti::compile_statement(function->body, compiler);
        }
        
        compiler.emit_label(fmt::format("@function_end_{}:\n", name));
        
        //issue functionreturn
        compiler.emit_pop(RegisterAllocation{ false, RegisterType::IP, compiler });
    }
}
