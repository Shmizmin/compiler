#include "Statement.hpp"
#include "Function.hpp"
#include "Context.hpp"
#include "Error.hpp"
#include "Central.hpp"

#include <algorithm>


void ti::stmt::Block::generate(ti::Context& context, ti::Function& function) noexcept
{
    ti::write_log("Generating code for block statement");

    std::for_each(statements.begin(), statements.end(), [&](Statement* statement)
    {
        statement->generate(context, function);
    });
}

void ti::stmt::If::generate(ti::Context& context, ti::Function& function) noexcept
{
    ti::write_log("Generating code for if statement");
    
    const auto label = ti::format("if_end_%s_%s", function.name.c_str(), context.counter);
    const auto alloc = context.allocate_forced();
    
    condition->generate(context, function, alloc);
    
    context.add_to_code("\tupdateFlags()\n");
    context.add_to_code(ti::format("\tjez %s", label.c_str()));
    
    context.deallocate_forced(alloc);
    
    statement->generate(context, function);
    
    context.add_to_code(ti::format("@%s:\n", label.c_str()));
}

void ti::stmt::While::generate(ti::Context& context, ti::Function& function) noexcept
{
    ti::write_log("Generating code for while statement");
    
    const auto label = ti::format("@while_begin_%s_%s:\n", function.name.c_str(), context.counter);
    const auto alloc = context.allocate_forced();
    
    statement->generate(context, function);
    condition->generate(context, function, alloc);
    
    context.deallocate_forced(alloc);
    
    context.add_to_code("\tupdateFlags()\n");
    context.add_to_code(ti::format("\tjnz(%s)\n", label.c_str()));
}

void ti::stmt::Return::generate(ti::Context& context, ti::Function& function) noexcept
{
    ti::write_log("Generating code for return statement");

    const auto alloc = context.allocate_forced();
    
    value->generate(context, function, alloc);
    
    //for specialized call macro
    context.add_to_code(ti::format("\tpush %s\n", ti::location_to_string(alloc.location).c_str()));
    
    context.add_to_code(ti::format("\tjmp(function_end_%s)\n", function.name.c_str()));
    
    context.deallocate_forced(alloc);
}

void ti::stmt::Null::generate(ti::Context& context, ti::Function& function) noexcept
{
    ti::write_log("Generating code for null statement");
}

void ti::stmt::Variable::generate(ti::Context& context, ti::Function& function) noexcept
{
    ti::write_log("Generating code for variable statement");

    std::for_each(variables.begin(), variables.end(), [&](ti::Variable* variable)
    {
        const auto& name = variable->name;
        const auto  defined = (variable->value != nullptr);
        
        if (variable->type.specifier == ti::TypeSpecifier::VOID)
        {
            ti::throw_error("Variable %s was illegally declared as type \'void\'", name.c_str());
        }

        const auto address = context.allocate_heap(ti::get_type_size(variable->type));
              auto* symbol = new ti::VariableSymbol
        {
            {
                .type = ti::SymbolType::VARIABLE,
                .name = name,
                .defined = defined,
            },
            
            address,
            variable->visibility,
            function,
        };
    
        context.add_to_symbol_table(symbol);
        
        if (defined)
        {
            const auto alloc = context.allocate_forced();
            
            variable->value->generate(context, function, alloc);
            
            context.add_to_code(ti::format("\tstb %%%u, %s\n", address, ti::location_to_string(alloc.location).c_str()));
                                
            context.deallocate_forced(alloc);
        }
    
    });
}
