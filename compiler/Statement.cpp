#include "Statement.hpp"
#include "Function.hpp"
#include "Context.hpp"
#include "Error.hpp"
#include "Central.hpp"

#include <algorithm>

namespace
{
    void compile_block(const ti::stmt::Block& block, ti::CommonArgs& common) noexcept
    {
        ti::write_log("Compiling block statement");
        
        for (const auto statement : block.statements)
        {
            ti::compile_statement(statement, common);
        }
    }
    
    void compile_if(const ti::stmt::If& ifs, ti::CommonArgs& common) noexcept
    {
        ti::write_log("Compiling if statement");
        
        const auto label_end = ti::format("if_end_%s_%s", common.parent_function.name, ti::generate_uuid());
        
        {
            ti::ForcedRegisterAllocation new_allocation{ common.context };
            
            ti::compile_expression(ifs.condition, common2);
        
            common.context.emit_adc(new_allocation.location, 0);
            common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, label_end);
        }
        
        ti::compile_statement(ifs.statement, common);
        
        common.context.emit_label(label_end);
    }
    
    void compile_while(const ti::stmt::While& whiles, ti::CommonArgs& common) noexcept
    {
        ti::write_log("Compiling while statement");
        
        const auto label_template = "%s_%s_%s_%u";
        
        const auto  begin_label = ti::format(label_template, "while", "begin",  common.parent_function.name, ti::generate_uuid()),
                   oneify_label = ti::format(label_template, "while", "oneify", common.parent_function.name, ti::generate_uuid()),
                      end_label = ti::format(label_template, "while", "end",    common.parent_function.name, ti::generate_uuid());
        
        ti::compile_statement(whiles.statement, common);
        ti::compile_expression(whiles.condition, common);
        
        if (whiles.condition->type == ti::ExpressionType::IDENTIFIER)
        {
            common.context.emit_adc(common.allocation, 0);
        }
        
        common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, oneify_label);
        common.context.emit_and(new_allocation.location, 0);
        common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, end_label);
        common.context.emit_label(oneify_label);
        common.context.emit_or(new_allocation.location, 1);
        common.context.emit_label(end_label);
    }
    
    void compile_return(const ti::stmt::Return& returns, ti::CommonArgs& common) noexcept
    {
        ti::write_log("Compiling return statement");
        
        ti::compile_expression(returns.value, common);
        
        common.context.emit_push(common.allocation);
        common.context.emit_and(common.allocation, 0);
        common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, ti::format("function_end_%s", common.parent_function.name));
    }
    
    void compile_variable(const ti::stmt::Variable& variables, ti::CommonArgs& common) noexcept
    {
        ti::write_log("Compiling variable declaration statement");
        
        for (const auto variable : variables.variables)
        {
            const auto& name = variable->name;
            const auto defined = (variable->value != nullptr);
            
            const auto address = common.context.allocate_heap(ti::get_size_by_type(variable->type));
            common.context.symbol_table.emplace_back(new ti::Symbol
            {
                .type = ti::SymbolType::VARIABLE,
                .name = name,
                .defined = defined,
                .as.variable = ti::sym::Variable
                {
                    .address = address,
                    .visibility = variable->visibility,
                    .parent_function = common.parent_function,
                },
            });
            
            if (defined)
            {
                ti::RegisterAllocation new_allocation{ context };
                
                ti::generate_expression(variable->value, common2);
                
                common.context.emit_stb(address, new_allocation.location);
            }
        }
    }
}

namespace ti
{
    void compile_statement(Statement* stmt, CommonArgs& common) noexcept
    {
        using enum StatementType;
        switch (stmt->type)
        {
            case BLOCK:    return compile_block   (stmt->as.block,    common); break;
            case IF:       return compile_if      (stmt->as.ifs,      common); break;
            case WHILE:    return compile_while   (stmt->as.whiles,   common); break;
            case RETURN:   return compile_return  (stmt->as.returns,  common); break;
            case VARIABLE: return compile_variable(stmt->as.variable, common); break;
        }
    }
    
    Statement* make_block(std::vector<Statement*>&& stmts) noexcept
    {
        return new Statement
        {
            .type = ti::StatementType::BLOCK,
            .as.block.statements = std::move(stmts),
        };
    }
    
    Statement* make_if(Expression* cond, Statement* stmt) noexcept
    {
        return new Statement
        {
            .type = ti::StatementType::IF,
            .as.ifs.condition = cond,
            .as.ifs.statement = stmt,
        };
    }
    
    Statement* make_while(Expression* cond, Statement* stmt) noexcept
    {
        return new Statement
        {
            .type = ti::StatementType::WHILE,
            .as.whiles.condition = cond,
            .as.whiles.statement = stmt,
        };
    }
    
    Statement* make_return(Expression* expr) noexcept
    {
        return new Statement
        {
            .type = ti::StatementType::RETURN,
            .as.returns.value = expr,
        };
    }
    
    Statement* make_variable(std::vector<Variable*>&& vars) noexcept
    {
        return new Statement
        {
            .type = ti::StatementType::VARIABLE,
            .as.variable.variables = std::move(vars),
        };
    }
}


/*
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
                .defined = true,
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
*/
