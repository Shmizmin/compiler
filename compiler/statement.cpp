#include "statement.hpp"
#include "function.hpp"
#include "compiler.hpp"
#include "error.hpp"
#include "allocation.hpp"
#include "types.hpp"

#include <fmt/format.h>

namespace
{
    void compile_block(ti::Statement* statement, ti::Compiler& compiler, ti::RegisterAllocation allocation) noexcept
    {
        const auto& block = statement->as.block;
        
        for (const auto stmt : block.statements)
        {
            ti::compile_statement(stmt, compiler);
        }
    }
    
    void compile_if(ti::Statement* statement, ti::Compiler& compiler, ti::RegisterAllocation allocation) noexcept
    {
        const auto& ifs = statement->as.ifs;
        
        auto label_end = fmt::format("if_end_{}_{}", statement->parent_function->name, ti::generate_uuid());
        
        ti::compile_expression(ifs.condition, compiler, allocation);
        
        compiler.emit_adc(allocation, 0);
        compiler.emit_jmp(ti::insn::Jmp::Condition::JEZ, label_end);
        
        ti::compile_statement(ifs.statement, compiler);
        
        compiler.emit_label(label_end);
    }
    
    void compile_while(ti::Statement* statement, ti::Compiler& compiler, ti::RegisterAllocation allocation) noexcept
    {
        const auto& whiles = statement->as.whiles;
        
        const auto label_template = "{}_{}_{}_{}";
        
        auto  begin_label = fmt::format(label_template, "while", "begin",  statement->parent_function->name, ti::generate_uuid()),
             oneify_label = fmt::format(label_template, "while", "oneify", statement->parent_function->name, ti::generate_uuid()),
                end_label = fmt::format(label_template, "while", "end",    statement->parent_function->name, ti::generate_uuid());
        
        ti::compile_statement(whiles.statement, compiler);
        ti::compile_expression(whiles.condition, compiler, allocation);
        
        if (whiles.condition->type == ti::ExpressionType::IDENTIFIER)
        {
            compiler.emit_adc(allocation, 0);
        }
        
        compiler.emit_jmp(ti::insn::Jmp::Condition::JEZ, oneify_label);
        compiler.emit_and(allocation, 0);
        compiler.emit_jmp(ti::insn::Jmp::Condition::JEZ, end_label);
        compiler.emit_label(oneify_label);
        compiler.emit_lor(allocation, 1);
        compiler.emit_label(end_label);
    }
    
    void compile_return(ti::Statement* statement, ti::Compiler& compiler, ti::RegisterAllocation allocation) noexcept
    {
        const auto& returns = statement->as.returns;
        
        ti::compile_expression(returns.value, compiler, allocation);
        
        compiler.emit_push(allocation);
        compiler.emit_and(allocation, 0);
        compiler.emit_jmp(ti::insn::Jmp::Condition::JEZ, fmt::format("function_end_{}", statement->parent_function->name));
    }
    
    void compile_variable(ti::Statement* statement, ti::Compiler& compiler, ti::RegisterAllocation allocation) noexcept
    {
        const auto& variables = statement->as.variable;
        
        for (const auto variable : variables.variables)
        {
            const auto& name = variable->name;
            const auto defined = (variable->value != nullptr);
            
            const auto heap_allocation = ti::HeapAllocation{ compiler, ti::get_size_by_type(variable->type) };

            compiler.add_to_symbol_table(new ti::Symbol
            {
                .type = ti::SymbolType::VARIABLE,
                .name = name,
                .defined = defined,
                .as.variable = ti::sym::Variable
                {
                    .address = heap_allocation.address(),
                    .visibility = variable->visibility,
                    .parent_function = ti::compiling_function,
                },
            });
            
            if (defined)
            {
                ti::RegisterAllocation new_allocation{ compiler };
                
                ti::compile_expression(variable->value, compiler, new_allocation);
                
                compiler.emit_stb(heap_allocation, new_allocation);
            }
        }
    }
}


namespace ti
{
    void compile_statement(Statement* statement, ti::Compiler& compiler) noexcept
    {
        ti::RegisterAllocation new_allocation{ compiler };
        
        using enum StatementType;
        switch (statement->type)
        {
            case BLOCK:
            {
                ti::write_log("Compiling block statement");
                return ::compile_block(statement, compiler, new_allocation);
            } break;
                
            case IF:
            {
                ti::write_log("Compiling if statement");
                return ::compile_if(statement, compiler, new_allocation);
            } break;
                
            case WHILE:
            {
                ti::write_log("Compiling while statement");
                return ::compile_while(statement, compiler, new_allocation);
            } break;
                
            case RETURN:
            {
                ti::write_log("Compiling return statement");
                return ::compile_return(statement, compiler, new_allocation);
            } break;
                
            case VARIABLE:
            {
                ti::write_log("Compiling variable statement");
                return ::compile_variable(statement, compiler, new_allocation);
            } break;
        }
    }
    
    
    Statement* make_block(std::vector<Statement*>&& statements) noexcept
    {
        return new Statement
        {
            .type = ti::StatementType::BLOCK,
            .as.block.statements = std::move(statements),
        };
    }
    
    Statement* make_if(Expression* condition, Statement* statement) noexcept
    {
        return new Statement
        {
            .type = ti::StatementType::IF,
            .as.ifs.condition = condition,
            .as.ifs.statement = statement,
            .parent_function = compiling_function,
        };
    }
    
    Statement* make_while(Expression* condition, Statement* statement) noexcept
    {
        return new Statement
        {
            .type = ti::StatementType::WHILE,
            .as.whiles.condition = condition,
            .as.whiles.statement = statement,
            .parent_function = compiling_function,
        };
    }
    
    Statement* make_return(Expression* expression) noexcept
    {
        return new Statement
        {
            .type = ti::StatementType::RETURN,
            .as.returns.value = expression,
            .parent_function = compiling_function,
        };
    }
    
    Statement* make_variable(std::vector<Variable*>&& variables) noexcept
    {
        return new Statement
        {
            .type = ti::StatementType::VARIABLE,
            .as.variable.variables = std::move(variables),
            .parent_function = compiling_function,
        };
    }
}
