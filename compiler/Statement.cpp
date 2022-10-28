#include "Statement.hpp"
#include "Function.hpp"
#include "Context.hpp"
#include "Error.hpp"
#include "Central.hpp"
#include "Allocation.hpp"
#include "Types.hpp"

#include <fmt/format.h>

namespace
{
    void compile_block(const ti::stmt::Block& block, ti::CommonArgs& common) noexcept
    {
        for (const auto statement : block.statements)
        {
            ti::compile_statement(statement, common);
        }
    }
    
    void compile_if(const ti::stmt::If& ifs, ti::CommonArgs& common) noexcept
    {
        auto label_end = fmt::format("if_end_{}_{}", common.parent_function.name, ti::generate_uuid());
        
        {
            ti::RegisterAllocation new_allocation{ common.context };
            
            ti::CommonArgs args common2;
            ti::compile_expression(ifs.condition, args);
        
            common.context.emit_adc(new_allocation.location, 0);
            common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, label_end);
        }
        
        ti::compile_statement(ifs.statement, common);
        
        common.context.emit_label(label_end);
    }
    
    void compile_while(const ti::stmt::While& whiles, ti::CommonArgs& common) noexcept
    {
        const auto label_template = "{}_{}_{}_{}";
        
        auto  begin_label = fmt::format(label_template, "while", "begin",  common.parent_function.name, ti::generate_uuid()),
             oneify_label = fmt::format(label_template, "while", "oneify", common.parent_function.name, ti::generate_uuid()),
                end_label = fmt::format(label_template, "while", "end",    common.parent_function.name, ti::generate_uuid());
        
        ti::compile_statement(whiles.statement, common);
        ti::compile_expression(whiles.condition, common);
        
        if (whiles.condition->type == ti::ExpressionType::IDENTIFIER)
        {
            common.context.emit_adc(common.allocation, 0);
        }
        
        common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, oneify_label);
        common.context.emit_and(common.allocation, 0);
        common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, end_label);
        common.context.emit_label(oneify_label);
        common.context.emit_lor(common.allocation, 1);
        common.context.emit_label(end_label);
    }
    
    void compile_return(const ti::stmt::Return& returns, ti::CommonArgs& common) noexcept
    {
        ti::compile_expression(returns.value, common);
        
        common.context.emit_push(common.allocation);
        common.context.emit_and(common.allocation, 0);
        common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, fmt::format("function_end_{}", common.parent_function.name));
    }
    
    void compile_variable(const ti::stmt::Variables& variables, ti::CommonArgs& common) noexcept
    {
        for (const auto variable : variables.variables)
        {
            const auto& name = variable->name;
            const auto defined = (variable->value != nullptr);
            
            const auto heap_allocation = ti::HeapAllocation(common.context, ti::get_size_by_type(variable->type));

            common.context.symbol_table.emplace_back(new ti::Symbol
            {
                .type = ti::SymbolType::VARIABLE,
                .name = name,
                .defined = defined,
                .as.variable = ti::sym::Variable
                {
                    .address = heap_allocation.address,
                    .visibility = variable->visibility,
                    .parent_function = common.parent_function,
                },
            });
            
            if (defined)
            {
                ti::RegisterAllocation new_allocation{ common.context };
                
                ti::CommonArgs args common2;
                ti::compile_expression(variable->value, args);
                
                common.context.emit_stb(heap_allocation.address, new_allocation.location);
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
            case BLOCK:
            {
                ti::write_log("Compiling block statement");
                return ::compile_block(stmt->as.block, common);
            } break;
                
            case IF:
            {
                ti::write_log("Compiling if statement");
                return ::compile_if(stmt->as.ifs, common);
            } break;
                
            case WHILE:
            {
                ti::write_log("Compiling while statement");
                return ::compile_while(stmt->as.whiles, common);
            } break;
                
            case RETURN:
            {
                ti::write_log("Compiling return statement");
                return ::compile_return(stmt->as.returns, common);
            } break;
                
            case VARIABLE:
            {
                ti::write_log("Compiling variable statement");
                return ::compile_variable(stmt->as.variable, common);
            } break;
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
