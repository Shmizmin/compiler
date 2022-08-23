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
    
    const auto label = ti::format("@if_end_%s_%s:", function.name.c_str(), context.counter);
    const auto alloc = context.force_allocate();
    
    condition->generate(context, function, alloc);
    
    context.add_to_code("updateFlags()\n");
    context.add_to_code(ti::format("jez [%s]", label));
    
    context.force_deallocate(alloc);
    
    statement->generate(context, function);
    
    context.add_to_code(label);
}

void ti::WhileStatement::generate(ti::Context& context, ti::Function& function) noexcept
{
    ti::write_log("Generating code for while statement");
}

void ti::ReturnStatement::generate(ti::Context& context, ti::Function& function) noexcept
{
    ti::write_log("Generating code for return statement");
}

void ti::NilStatement::generate(ti::Context& context, ti::Function& function) noexcept
{
    //ti::write_log("Generating code for null statement");
}

void ti::VariableStatemnet::generate(ti::Context& context, ti::Function& function) noexcept
{
    ti::write_log("Generating code for variable statement");
}
