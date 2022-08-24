#include "Statement.hpp"
#include "Function.hpp"
#include "Context.hpp"
#include "Error.hpp"
#include "Driver.hpp"

#include <algorithm>


void ti::BlockStatement::generate(ti::Context& context, ti::Function& function) noexcept
{
    ti::write_log("Generating code for block statement");

    std::for_each(statements.begin(), statements.end(), [&](Statement* statement)
    {
        statement->generate(context, function);
    });
}

void ti::IfStatement::generate(ti::Context& context, ti::Function& function) noexcept
{
    ti::write_log("Generating code for if statement");
    
    const auto label = ti::format("@if_end_%s_%s:\n", function.name.c_str(), context.counter);
    const auto alloc = context.allocate_forced();
    
    condition->generate(context, function, alloc);
    
    context.add_to_code("updateFlags()\n");
    context.add_to_code(ti::format("jez [%s]", label));
    
    context.deallocate_forced(alloc);
    
    statement->generate(context, function);
    
    context.add_to_code(label);
}

void ti::WhileStatement::generate(ti::Context& context, ti::Function& function) noexcept
{
    ti::write_log("Generating code for while statement");
    
    const auto label = ti::format("@while_begin_%s_%s:\n", function.name.c_str(), context.counter);
    const auto alloc = context.allocate_forced();
    
    statement->generate(context, function);
    condition->generate(context, function, alloc);
    
    context.deallocate_forced(alloc);
    
    context.add_to_code("updateFlags()\n");
    context.add_to_code(ti::format("jnz([%s])\n", label.c_str()));
}

void ti::ReturnStatement::generate(ti::Context& context, ti::Function& function) noexcept
{
    ti::write_log("Generating code for return statement");

    value->generate(context, function, { ti::Location::STACK, false });
    context.add_to_code(ti::format("jmp([%s_end])\n", function.name.c_str()));
}

void ti::NilStatement::generate(ti::Context& context, ti::Function& function) noexcept
{
    ti::write_log("Generating code for null statement");
}

void ti::VariableStatemnet::generate(ti::Context& context, ti::Function& function) noexcept
{
    ti::write_log("Generating code for variable statement");
    
    std::for_each(variables.begin(), variables.end(), [&](Variable& variable)
    {
        const auto& name = variable.name;
        const auto  defined = (variable.value != nullptr);
        
        if (variable.type.specifier == ti::TypeSpecifier::VOID)
        {
            ti::throw_error("Variable %s was illegally declared as type \'void\'", name);
        }
        
        const auto address = context.allocate_heap(variable.type);
              auto* symbol = new ti::VariableSymbol
        {
            {
                .type = ti::SymbolType::VARIABLE,
                .name = name,
                .defined = defined,
            },
            
            address,
            variable.visibility,
            function,
        };
    
        context.add_to_symbol_table(symbol);
        
        //only supports 1 byte values for now
        //should the variable expression be code generated
#warning ONLY ONE BYTE VALUES ARE SUPPORTED FOR VARIABLES
        if (defined)
        {
            const auto alloc = context.allocate_forced();
            
            variable.value->generate(context, function, alloc);
            
            context.add_to_code(ti::format("stb %%%s, %s\n", address, alloc.location));
                                
            context.deallocate_forced(alloc);
        }
    
    });
}
