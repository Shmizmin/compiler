#include "Expression.hpp"

#include "Context.hpp"
#include "Function.hpp"
#include "Error.hpp"
#include "Central.hpp"
#include "Bytecode.hpp"

#include <iostream>
#include <stdexcept>
#include <exception>
#include <fmt/format.h>

namespace
{
    bool funcs_equal(ti::Function& f1, ti::Function& f2)
    {
        bool args_same = (f1.arguments.size() == f2.arguments.size());
        
        if (args_same)
        {
            for (auto i = 0; i < f1.arguments.size(); ++i)
            {
                const auto& arg1 = f1.arguments[i],
                            arg2 = f2.arguments[i];
                
                if ((arg1.type.specifier != arg2.type.specifier) ||
                    (arg1.type.qualifier != arg2.type.qualifier) ||
                    (arg1.name !=  arg2.name))
                {
                    args_same = false;
                }
            }
        }
        
        return (f1.name                  == f2.name)                  &&
               (f1.body                  == f2.body)                  &&
               (f1.return_type.specifier == f2.return_type.specifier) &&
               (f1.return_type.qualifier == f2.return_type.qualifier) && args_same;
    }
}
    
namespace
{
#define symbols common.context.symbol_table
    void compile_numconst(const ti::expr::Numconst& numconst, ti::CommonArgs& common) noexcept
    {
        ti::write_log("\t\tCompiling 8-bit numeric constant expression");
        
        common.context.emit_ldb(common.allocation, numconst.value);
    }
    
    void compile_stringconst(const ti::expr::Stringconst& stringconst, ti::CommonArgs& common) noexcept
    {
        ti::write_log("\t\tCompiling string constant expression");
        
        const auto ptr = common.context.allocate_heap(stringconst.text.size());
        
        // .org ptr
        common.context.emit_org(ptr);
        
        // .ascii "text"
        common.context.emit_ascii(stringconst.text);

        // push rx
        common.context.emit_ldb(common.allocation, ptr & 0x00FF);
        common.context.emit_push(common.allocation);
        
        common.context.emit_ldb(common.allocation, ptr & 0xFF00);
        common.context.emit_push(common.allocation);
    }
    
    void compile_identifier(const ti::expr::Identifier& identifier, ti::CommonArgs& common) noexcept
    {
        ti::write_log("\t\tCompiling identifier expression");
            
        for (const auto symbol : common.context.symbol_table)
        {
            //add in error checking and messages for each specific case here
            if (symbol->defined &&
                symbol->type == ti::SymbolType::VARIABLE &&
                symbol->name == identifier.name)
            {
                if (symbol->as.variable.visibility == ti::TypeVisibility::LOCAL &&
                    !funcs_equal(symbol->as.variable.parent_function, common.parent_function))
                {
                    //throw error maybe?
                }
                
                //found the variable we were looking for
                common.context.emit_ldb(common.allocation, symbol->as.variable.address);
                return;
            }
        }
    }
    
    void compile_function_call(const ti::expr::Function_Call& function_call, ti::CommonArgs& common) noexcept
    {
        ti::write_log("\t\tCompiling function call expression");
        
        for (const auto symbol : symbols)
        {
            auto defined_arity = symbol->as.function.arguments.size();
            
            if (defined_arity == function_call.args.size() &&
                symbol->defined &&
                symbol->type == ti::SymbolType::FUNCTION &&
                symbol->name == function_call.left.name)
            {
                //found the function we were looking for
                for (auto i = 0u; i < defined_arity; ++i)
                {
                    for (const auto argument_symbol : symbols)
                    {
                        // TODO: verify that we dont need to check other fields of the 'Symbol' struct to verify equality
                        if (argument_symbol->name == fmt::format("{}_{}", function_call.left.name, symbol->as.function.arguments[i].name))
                        {
                            argument_symbol->defined = true;
                            
                            ti::compile_expression(function_call.args[i], common);
                            
                            common.context.emit_stb(argument_symbol->as.variable.address, common.allocation);
                        }
                    }
                }
                
                //first arg is call_rx (return location of function)
                common.context.emit_call(common.allocation, fmt::format("function_start_{}", symbol->name));
            }
        }
    }
    
    void compile_ternaryop(const ti::expr::Ternaryop& ternaryop, ti::CommonArgs& common) noexcept
    {
        ti::write_log("\t\tCompiling ternary expresssion");
                    
        ti::compile_expression(ternaryop.left, common);
                    
        //verify that we can perform a conditional jump on the expression with valid flags
        //assume it is a register for now
        common.context.emit_adc(common.allocation, 0);
                   
        const auto label_template = "ternary_{}_{}_{}";
                    
        const auto  true_label = fmt::format(label_template, common.parent_function.name, "true",  ti::generate_uuid()),
                   false_label = fmt::format(label_template, common.parent_function.name, "false", ti::generate_uuid()),
                     end_label = fmt::format(label_template, common.parent_function.name, "end",   ti::generate_uuid());
                    
        common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, false_label);
                    
        common.context.emit_label(true_label);
        ti::compile_expression(ternaryop.center, common);
                    
        common.context.emit_lor(common.allocation, 1);
        common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, end_label);
        
        common.context.emit_label(false_label);
        ti::compile_expression(ternaryop.right, common);
        
        common.context.emit_label(end_label);
        
    }
    
    void compile_binaryop(const ti::expr::Binaryop& binaryop, ti::CommonArgs& common) noexcept
    {
        ti::write_log("\t\tCompiling binary operator expression");
        
        using enum ti::BinaryOperator;
        switch (binaryop.type)
        {
            case EQUALS:
            {
                ti::write_log("\t\t\tCompiling assignment binary operator");
                
                if (binaryop.left->type == ti::ExpressionType::IDENTIFIER)
                {
                    for (auto symbol : symbols)
                    {
                        if (symbol->type == ti::SymbolType::VARIABLE &&
                            symbol->name == binaryop.left->as.identifier.name)
                        {
                            if (symbol->as.variable.visibility == ti::TypeVisibility::LOCAL &&
                                !funcs_equal(symbol->as.variable.parent_function, common.parent_function))
                            {
                                //throw error maybe?
                            }
                            
                            ti::compile_expression(binaryop.right, common);
                            common.context.emit_stb(symbol->as.variable.address, common.allocation);
                        }
                    }
                }
            } break;
                
            case PLUS:
            {
                ti::write_log("\t\t\tCompiling addition binary operator");
                
                ti::compile_expression(binaryop.left, common);
                
                ti::ForcedRegisterAllocation new_allocation{ common.context, common.allocation };
                ti::CommonArgs args common2;
                
                ti::compile_expression(binaryop.right, args);
                common.context.emit_adc(common.allocation, new_allocation.location);
                
            } break;
                
            case MINUS:
            {
                ti::write_log("\t\t\tCompiling subtraction binary operator");
                
                ti::ForcedRegisterAllocation new_allocation{ common.context, common.allocation };
                ti::CommonArgs args common2;
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, args);
                
                common.context.emit_sbb(common.allocation, new_allocation.location);
            } break;
                
            case LEFT_SHIFT:
            {
                ti::write_log("\t\t\tCompiling left shift binary operator");
                
                //once constant expressions are implemented, more than just numbers can be on right hand side
                //ie [5 + 1 << 3] may be considered a valid rvalue
                if (binaryop.right->type == ti::ExpressionType::NUMCONST)
                {
                    ti::compile_expression(binaryop.left, common);
                    common.context.emit_rol(common.allocation, binaryop.right->as.numconst.value);
                }
            } break;
                
            case RIGHT_SHIFT:
            {
                ti::write_log("\t\t\tCompiling right shift binary operator");
                
                //see left shift for later improvements
                if (binaryop.right->type == ti::ExpressionType::NUMCONST)
                {
                    ti::compile_expression(binaryop.left, common);
                    common.context.emit_ror(common.allocation, binaryop.right->as.numconst.value);
                }
            } break;
                
            case BIT_XOR:
            {
                ti::write_log("\t\t\tCompiling bitwise xor binary expression");
                
                ti::ForcedRegisterAllocation new_allocation{ common.context, common.allocation };
                ti::CommonArgs args common2;
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, args);
                
                common.context.emit_xor(common.allocation, new_allocation.location);
            } break;
                
            case BIT_AND:
            {
                ti::write_log("\t\t\tCompiling bitwise and binary expression");
                
                ti::ForcedRegisterAllocation new_allocation{ common.context, common.allocation };
                ti::CommonArgs args common2;
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, args);
                    
                common.context.emit_and(common.allocation, new_allocation.location);
            } break;
                
            case BIT_OR:
            {
                ti::write_log("\t\t\tCompiling bitwise or binary expression");
                
                ti::ForcedRegisterAllocation new_allocation{ common.context, common.allocation };
                ti::CommonArgs args common2;
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, args);
                
                common.context.emit_lor(common.allocation, new_allocation.location);
            } break;
                
            case EQUALS_EQUALS:
            {
                ti::write_log("\t\t\tCompiling equality test binary expression");
                
                ti::ForcedRegisterAllocation new_allocation{ common.context, common.allocation };
                ti::CommonArgs args common2;
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, args);
                
                // FIXME: maybe? zero flag will be set if equal
                common.context.emit_sbb(common.allocation, new_allocation.location);
            } break;
                
            case NOT_EQUALS:
            {
                ti::write_log("\t\t\tCompiling non-equality test binary expression");
                
                ti::ForcedRegisterAllocation new_allocation{ common.context, common.allocation };
                ti::CommonArgs args common2;
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, args);
                
                const auto label_template = "{}_{}_{}";
                
                const auto oneify_label = fmt::format(label_template, "oneify", common.parent_function.name, ti::generate_uuid()),
                              end_label = fmt::format(label_template, "end",    common.parent_function.name, ti::generate_uuid());
                
                // FIXME: maybe? zero flag will be set if not equal
                common.context.emit_sbb(common.allocation, new_allocation.location);
                
                common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, oneify_label);
                common.context.emit_and(common.allocation, 0);
                common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, end_label);
                common.context.emit_label(oneify_label);
                common.context.emit_lor(common.allocation, 1);
                common.context.emit_label(end_label);
            } break;
                
            case LESS:
            {
                ti::write_log("\t\t\tCompiling less-than binary expression");

                ti::ForcedRegisterAllocation new_allocation{ common.context, common.allocation };
                ti::CommonArgs args common2;
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, args);
                
                const auto label_template = "{}_{}_{}";
                
                const auto less_label = fmt::format(label_template, "less", common.parent_function.name, ti::generate_uuid()),
                            end_label = fmt::format(label_template, "end",  common.parent_function.name, ti::generate_uuid());
                
                common.context.emit_sbb(common.allocation, new_allocation.location);
                
                //if carry is set, first operand is less
                //if carry not set, first operand is greater or equal
                
                common.context.emit_jmp(ti::insn::Jmp::Condition::JCS, less_label);
                common.context.emit_and(common.allocation, 0);
                common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, end_label);
                common.context.emit_label(less_label);
                common.context.emit_lor(common.allocation, 1);
                common.context.emit_label(end_label);
            } break;
                
            case GREATER:
            {
                ti::write_log("\t\t\tCompiling greater-than binary expression");
                
                ti::ForcedRegisterAllocation new_allocation{ common.context, common.allocation };
                ti::CommonArgs args common2;
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, args);
                
                const auto label_template = "{}_{}_{}";
                
                const auto less_label = fmt::format(label_template, "greater", common.parent_function.name, ti::generate_uuid()),
                            end_label = fmt::format(label_template, "end",     common.parent_function.name, ti::generate_uuid());
                
                common.context.emit_sbb(common.allocation, new_allocation.location);
                
                //if carry is set, first operand is less
                //if carry not set, first operand is greater or equal
                
                common.context.emit_jmp(ti::insn::Jmp::Condition::JCS, less_label);
                common.context.emit_lor(common.allocation, 1);
                common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, end_label);
                common.context.emit_label(less_label);
                common.context.emit_and(common.allocation, 0);
                common.context.emit_label(end_label);
            } break;
        }
    }
    
    void compile_unaryop(const ti::expr::Unaryop& unaryop, ti::CommonArgs& common) noexcept
    {
        ti::write_log("\t\tCompiling unary operator expression");
        
        using enum ti::UnaryOperator;
        switch (unaryop.type)
        {
            case PLUS_PLUS:
            {
                ti::write_log("\t\t\tCompiling increment unary operator");
                
                ti::compile_expression(unaryop.center, common);
                common.context.emit_adc(common.allocation, 1);
            } break;
                
            case MINUS_MINUS:
            {
                ti::write_log("\t\t\tCompiling decrement unary operator");
                
                ti::compile_expression(unaryop.center, common);
                common.context.emit_sbb(common.allocation, 1);
            } break;
                
            case POSITIVE:
            {
                ti::write_log("\t\t\tCompiling positive unary operator");
                
                ti::compile_expression(unaryop.center, common);
            } break;
                
            case NEGATIVE:
            {
                ti::write_log("\t\t\tCompiling negation unary operator");
                
                ti::compile_expression(unaryop.center, common);
                
                common.context.emit_not(common.allocation);
                common.context.emit_adc(common.allocation, 1);
            } break;
        }
    }
#undef symbols
}

namespace ti
{
    void compile_expression(Expression* expr, CommonArgs& common) noexcept
    {
        using enum ExpressionType;
        switch (expr->type)
        {
            case NUMCONST:      return compile_numconst     (expr->as.numconst,     common); break;
            case STRINGCONST:   return compile_stringconst  (expr->as.stringconst,  common); break;
            case IDENTIFIER:    return compile_identifier   (expr->as.identifier,   common); break;
            case FUNCTION_CALL: return compile_function_call(expr->as.functioncall, common); break;
            case TERNARYOP:     return compile_ternaryop    (expr->as.ternaryop,    common); break;
            case BINARYOP:      return compile_binaryop     (expr->as.binaryop,     common); break;
            case UNARYOP:       return compile_unaryop      (expr->as.unaryop,      common); break;
        }
    }
    
    Expression* make_numconst(std::uint8_t value) noexcept
    {
        return new Expression
        {
            .type = ExpressionType::NUMCONST,
            .as.numconst = expr::Numconst{ value },
        };
    }
    
    Expression* make_stringconst(std::string&& text) noexcept
    {
        return new Expression
        {
            .type = ExpressionType::STRINGCONST,
            .as.stringconst = expr::Stringconst{ std::move(text) },
        };
    }
    
    Expression* make_identifier(std::string&& name) noexcept
    {
        return new Expression
        {
            .type = ExpressionType::IDENTIFIER,
            .as.identifier = expr::Identifier{ std::move(name) },
        };
    }
    
    Expression* make_function_call(std::string&& name, std::vector<Expression*>&& arguments) noexcept
    {
        return new Expression
        {
            .type = ExpressionType::FUNCTION_CALL,
            .as.functioncall.left = expr::Identifier{ std::move(name) },
            .as.functioncall.args = std::move(arguments),
        };
    }
    
    Expression* make_ternaryop(Expression* left, Expression* center, Expression* right) noexcept
    {
        return new Expression
        {
            .type = ExpressionType::TERNARYOP,
            .as.ternaryop.left = left,
            .as.ternaryop.center = center,
            .as.ternaryop.right = right,
        };
    }
    
    Expression* make_binaryop(Expression* left, Expression* right, BinaryOperator op) noexcept
    {
        return new Expression
        {
            .type = ExpressionType::BINARYOP,
            .as.binaryop.type = op,
            .as.binaryop.left = left,
            .as.binaryop.right = right,
        };
    }
    
    Expression* make_unaryop(Expression* center, UnaryOperator op) noexcept
    {
        return new Expression
        {
            .type = ExpressionType::UNARYOP,
            .as.unaryop.type = op,
            .as.unaryop.center = center,
        };
    }
}
