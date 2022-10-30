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
    void compile_function(Function* function, Compiler& context) noexcept
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
            ti::compile_statement(function->body, args);
        }
        
        context.emit_label(fmt::format("@function_end_{}:\n", name));
        
        //issue functionreturn
        context.emit_pop(RegisterType::IP);
    }
}
