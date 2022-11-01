#include "expression.hpp"
#include "allocation.hpp"
#include "compiler.hpp"
#include "function.hpp"
#include "error.hpp"
#include "bytecode.hpp"

#include <fmt/format.h>

namespace
{
    bool funcs_equal(const ti::Function* f1, const ti::Function* f2)
    {
        bool args_same = (f1->arguments.size() == f2->arguments.size());
        
        if (args_same)
        {
            for (auto i = 0; i < f1->arguments.size(); ++i)
            {
                const auto& arg1 = f1->arguments[i],
                            arg2 = f2->arguments[i];
                
                if ((arg1.type.specifier != arg2.type.specifier) ||
                    (arg1.type.qualifier != arg2.type.qualifier) ||
                    (arg1.name !=  arg2.name))
                {
                    args_same = false;
                }
            }
        }
        
        return (f1->name                  == f2->name)                  &&
               (f1->body                  == f2->body)                  &&
               (f1->return_type.specifier == f2->return_type.specifier) &&
               (f1->return_type.qualifier == f2->return_type.qualifier) && args_same;
    }
}
    
namespace
{
    void compile_numconst(ti::Expression* expression, ti::Compiler& compiler, const ti::RegisterAllocation& allocation) noexcept
    {
        const auto& numconst = expression->as.numconst;
        
        compiler.emit_ldb(allocation, numconst.value);
    }
    
    void compile_stringconst(ti::Expression* expression, ti::Compiler& compiler, const ti::RegisterAllocation& allocation) noexcept
    {
        const auto& stringconst = expression->as.stringconst;
        
        const auto heap_allocation = ti::HeapAllocation{ compiler, static_cast<std::uint16_t>(stringconst.text.size()) };
        
        compiler.emit_org(heap_allocation);
        compiler.emit_ascii(stringconst.text);
        
        compiler.emit_ldb(allocation, heap_allocation & 0x00FF);
        compiler.emit_push(allocation);
        
        compiler.emit_ldb(allocation, heap_allocation & 0xFF00);
        compiler.emit_push(allocation);
    }
    
    void compile_identifier(ti::Expression* expression, ti::Compiler& compiler, const ti::RegisterAllocation& allocation) noexcept
    {
        const auto& identifier = expression->as.identifier;
        
        for (const auto symbol : compiler.symbol_table)
        {
            //add in error checking and messages for each specific case here
            if (symbol->defined &&
                symbol->type == ti::SymbolType::VARIABLE &&
                symbol->name == identifier.name)
            {
                if (symbol->as.variable.visibility == ti::TypeVisibility::LOCAL &&
                    !funcs_equal(symbol->as.variable.parent_function, expression->parent_function))
                {
                    //ti::throw_error(fmt::format("Variable {} is local to function {}, not function {}", symbol->name, symbol->as.variable.parent_function.name, common.parent_function));
                    //throw error maybe?
                }
                
                //found the variable we were looking for
                compiler.emit_ldb(allocation, symbol->as.variable.address);
                return;
            }
        }
    }
    
    void compile_functioncall(ti::Expression* expression, ti::Compiler& compiler, const ti::RegisterAllocation& allocation) noexcept
    {
        const auto& function_call = expression->as.functioncall;
        
        for (const auto symbol : compiler.symbol_table)
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
                    for (const auto argument_symbol : compiler.symbol_table)
                    {
                        // TODO: verify that we dont need to check other fields of the 'Symbol' struct to verify equality
                        if (argument_symbol->name == fmt::format("{}_{}", function_call.left.name, symbol->as.function.arguments[i].name))
                        {
                            argument_symbol->defined = true;
                            
                            ti::compile_expression(function_call.args[i], compiler, allocation);
                            
                            compiler.emit_stb(argument_symbol->as.variable.address, allocation);
                        }
                    }
                }
                
                //first arg is call_rx (return location of function)
                compiler.emit_call(allocation, fmt::format("function_start_{}", symbol->name));
            }
        }
    }
    
    void compile_ternaryop(ti::Expression* expression, ti::Compiler& compiler, const ti::RegisterAllocation& allocation) noexcept
    {
        const auto& ternaryop = expression->as.ternaryop;
        
        ti::compile_expression(ternaryop.left, compiler, allocation);
                    
        //verify that we can perform a conditional jump on the expression with valid flags
        //assume it is a register for now
        compiler.emit_adc(allocation, 0);
                   
        const auto label_template = "ternary_{}_{}_{}";
                    
        const auto  true_label = fmt::format(label_template, expression->parent_function->name, "true",  ti::generate_uuid()),
                   false_label = fmt::format(label_template, expression->parent_function->name, "false", ti::generate_uuid()),
                     end_label = fmt::format(label_template, expression->parent_function->name, "end",   ti::generate_uuid());
                    
        compiler.emit_jmp(ti::insn::Jmp::Condition::JEZ, false_label);
                    
        compiler.emit_label(true_label);
        ti::compile_expression(ternaryop.center, compiler, allocation);
                    
        compiler.emit_lor(allocation, 1);
        compiler.emit_jmp(ti::insn::Jmp::Condition::JEZ, end_label);
        
        compiler.emit_label(false_label);
        ti::compile_expression(ternaryop.right, compiler, allocation);
        
        compiler.emit_label(end_label);
        
    }
    
    void compile_binaryop(ti::Expression* expression, ti::Compiler& compiler, const ti::RegisterAllocation& allocation) noexcept
    {
        const auto& binaryop = expression->as.binaryop;
        
        using enum ti::BinaryOperator;
        switch (binaryop.type)
        {
            case EQUALS:
            {
                ti::write_log("\t\tCompiling assignment binary operator");
                
                if (binaryop.left->type == ti::ExpressionType::IDENTIFIER)
                {
                    for (auto symbol : compiler.symbol_table)
                    {
                        if (symbol->type == ti::SymbolType::VARIABLE &&
                            symbol->name == binaryop.left->as.identifier.name)
                        {
                            if (symbol->as.variable.visibility == ti::TypeVisibility::LOCAL &&
                                !funcs_equal(symbol->as.variable.parent_function, expression->parent_function))
                            {
                                //throw error maybe?
                            }
                            
                            ti::compile_expression(binaryop.right, compiler, allocation);
                            compiler.emit_stb(symbol->as.variable.address, allocation);
                        }
                    }
                }
            } break;
                
            case PLUS:
            {
                ti::write_log("\t\tCompiling addition binary operator");
                
                ti::RegisterAllocation new_allocation{ compiler, allocation };
                
                ti::compile_expression(binaryop.left, compiler, allocation);
                ti::compile_expression(binaryop.right, compiler, new_allocation);
                
                compiler.emit_adc(allocation, new_allocation);
                
            } break;
                
            case MINUS:
            {
                ti::write_log("\t\tCompiling subtraction binary operator");
                
                ti::RegisterAllocation new_allocation{ compiler, allocation };
                
                ti::compile_expression(binaryop.left, compiler, allocation);
                ti::compile_expression(binaryop.right, compiler, new_allocation);
                
                compiler.emit_sbb(allocation, new_allocation);
            } break;
                
            case LEFT_SHIFT:
            {
                ti::write_log("\t\tCompiling left shift binary operator");
                
                //once constant expressions are implemented, more than just numbers can be on right hand side
                //ie [5 + 1 << 3] may be considered a valid rvalue
                if (binaryop.right->type == ti::ExpressionType::NUMCONST)
                {
                    ti::compile_expression(binaryop.left, compiler, allocation);
                    compiler.emit_rol(allocation, binaryop.right->as.numconst.value);
                }
            } break;
                
            case RIGHT_SHIFT:
            {
                ti::write_log("\t\tCompiling right shift binary operator");
                
                //see left shift for later improvements
                if (binaryop.right->type == ti::ExpressionType::NUMCONST)
                {
                    ti::compile_expression(binaryop.left, compiler, allocation);
                    compiler.emit_ror(allocation, binaryop.right->as.numconst.value);
                }
            } break;
                
            case BIT_AND:
            {
                ti::write_log("\t\tCompiling bitwise and binary expression");
                
                ti::RegisterAllocation new_allocation{ compiler, allocation };
                
                ti::compile_expression(binaryop.left, compiler, allocation);
                ti::compile_expression(binaryop.right, compiler, new_allocation);
                    
                compiler.emit_and(allocation, new_allocation);
            } break;
                
            case BIT_OR:
            {
                ti::write_log("\t\tCompiling bitwise or binary expression");
                
                ti::RegisterAllocation new_allocation{ compiler, allocation };
                
                ti::compile_expression(binaryop.left, compiler, allocation);
                ti::compile_expression(binaryop.right, compiler, new_allocation);
                
                compiler.emit_lor(allocation, new_allocation);
            } break;
                
            case EQUALS_EQUALS:
            {
                ti::write_log("\t\tCompiling equality test binary expression");
                
                ti::RegisterAllocation new_allocation{ compiler, allocation };
                
                ti::compile_expression(binaryop.left, compiler, allocation);
                ti::compile_expression(binaryop.right, compiler, new_allocation);
                
                // FIXME: maybe? zero flag will be set if equal
                compiler.emit_sbb(allocation, new_allocation);
            } break;
                
            case NOT_EQUALS:
            {
                ti::write_log("\t\tCompiling non-equality test binary expression");
                
                ti::RegisterAllocation new_allocation{ compiler, allocation };
                
                ti::compile_expression(binaryop.left, compiler, allocation);
                ti::compile_expression(binaryop.right, compiler, new_allocation);
                
                const auto label_template = "{}_{}_{}";
                
                const auto oneify_label = fmt::format(label_template, "oneify", expression->parent_function->name, ti::generate_uuid()),
                              end_label = fmt::format(label_template, "end",    expression->parent_function->name, ti::generate_uuid());
                
                // FIXME: maybe? zero flag will be set if not equal
                compiler.emit_sbb(allocation, new_allocation);
                
                compiler.emit_jmp(ti::insn::Jmp::Condition::JEZ, oneify_label);
                compiler.emit_and(allocation, 0);
                compiler.emit_jmp(ti::insn::Jmp::Condition::JEZ, end_label);
                compiler.emit_label(oneify_label);
                compiler.emit_lor(allocation, 1);
                compiler.emit_label(end_label);
            } break;
                
            case LESS:
            {
                ti::write_log("\t\tCompiling less-than binary expression");

                ti::RegisterAllocation new_allocation{ compiler, allocation };
                
                ti::compile_expression(binaryop.left, compiler, allocation);
                ti::compile_expression(binaryop.right, compiler, new_allocation);
                
                const auto label_template = "{}_{}_{}";
                
                const auto less_label = fmt::format(label_template, "less", expression->parent_function->name, ti::generate_uuid()),
                            end_label = fmt::format(label_template, "end",  expression->parent_function->name, ti::generate_uuid());
                
                compiler.emit_sbb(allocation, new_allocation);
                
                //if carry is set, first operand is less
                //if carry not set, first operand is greater or equal
                
                compiler.emit_jmp(ti::insn::Jmp::Condition::JCS, less_label);
                compiler.emit_and(allocation, 0);
                compiler.emit_jmp(ti::insn::Jmp::Condition::JEZ, end_label);
                compiler.emit_label(less_label);
                compiler.emit_lor(allocation, 1);
                compiler.emit_label(end_label);
            } break;
                
            case GREATER:
            {
                ti::write_log("\t\tCompiling greater-than binary expression");
                
                ti::RegisterAllocation new_allocation{ compiler, allocation };
                
                ti::compile_expression(binaryop.left, compiler, allocation);
                ti::compile_expression(binaryop.right, compiler, new_allocation);
                
                const auto label_template = "{}_{}_{}";
                
                const auto less_label = fmt::format(label_template, "greater", expression->parent_function->name, ti::generate_uuid()),
                            end_label = fmt::format(label_template, "end",     expression->parent_function->name, ti::generate_uuid());
                
                compiler.emit_sbb(allocation, new_allocation);
                
                //if carry is set, first operand is less
                //if carry not set, first operand is greater or equal
                
                compiler.emit_jmp(ti::insn::Jmp::Condition::JCS, less_label);
                compiler.emit_lor(allocation, 1);
                compiler.emit_jmp(ti::insn::Jmp::Condition::JEZ, end_label);
                compiler.emit_label(less_label);
                compiler.emit_and(allocation, 0);
                compiler.emit_label(end_label);
            } break;
        }
    }

    void compile_unaryop(ti::Expression* expression, ti::Compiler& compiler, const ti::RegisterAllocation& allocation) noexcept
    {
        const auto& unaryop = expression->as.unaryop;
        
        using enum ti::UnaryOperator;
        switch (unaryop.type)
        {
            case PLUS_PLUS:
            {
                ti::write_log("\t\tCompiling increment unary operator");
                
                ti::compile_expression(unaryop.center, compiler, allocation);
                compiler.emit_adc(allocation, 1);
            } break;
                
            case MINUS_MINUS:
            {
                ti::write_log("\t\tCompiling decrement unary operator");
                
                ti::compile_expression(unaryop.center, compiler, allocation);
                compiler.emit_sbb(allocation, 1);
            } break;
                
            case NEGATIVE:
            {
                ti::write_log("\t\tCompiling negation unary operator");
                
                ti::compile_expression(unaryop.center, compiler, allocation);
                compiler.emit_not(allocation);
                compiler.emit_adc(allocation, 1);
            } break;
        }
    }
#undef symbols
}

namespace ti
{
    void compile_expression(Expression* expression, ti::Compiler& compiler, const ti::RegisterAllocation& allocation) noexcept
    {
        using enum ExpressionType;
        switch (expression->type)
        {
            case NUMCONST:
            {
                ti::write_log("\tCompiling numeric constant expression");
                return compile_numconst(expression, compiler, allocation);
            } break;
                
            case STRINGCONST:
            {
                ti::write_log("\tCompiling string constant expression");
                return compile_stringconst(expression, compiler, allocation);
            } break;
                
            case IDENTIFIER:
            {
                ti::write_log("\tCompiling identifier expression");
                return compile_identifier(expression, compiler, allocation);
            } break;
                
            case FUNCTIONCALL:
            {
                ti::write_log("\tCompiling function call expression");
                return compile_functioncall(expression, compiler, allocation);
            } break;
            
            case TERNARYOP:
            {
                ti::write_log("\tCompiling ternary operator expression");
                return compile_ternaryop(expression, compiler, allocation);
            } break;
                
            case BINARYOP:
            {
                ti::write_log("\tCompiling binary operator expression");
                return compile_binaryop(expression, compiler, allocation);
            } break;
                
            case UNARYOP:
            {
                ti::write_log("\tCompiling unary operator expression");
                return compile_unaryop(expression, compiler, allocation);
            } break;
        }
    }
    
    
    Expression* make_numconst(std::uint8_t value) noexcept
    {
        return new Expression
        {
            .type = ExpressionType::NUMCONST,
            .as.numconst = expr::Numconst{ value },
            .parent_function = compiling_function,
        };
    }
    
    Expression* make_stringconst(std::string&& text) noexcept
    {
        return new Expression
        {
            .type = ExpressionType::STRINGCONST,
            .as.stringconst = expr::Stringconst{ std::move(text) },
            .parent_function = compiling_function,
        };
    }
    
    Expression* make_identifier(std::string&& name) noexcept
    {
        return new Expression
        {
            .type = ExpressionType::IDENTIFIER,
            .as.identifier = expr::Identifier{ std::move(name) },
            .parent_function = compiling_function,
        };
    }
    
    Expression* make_functioncall(std::string&& name, std::vector<Expression*>&& arguments) noexcept
    {
        return new Expression
        {
            .type = ExpressionType::FUNCTIONCALL,
            .as.functioncall.left = expr::Identifier{ std::move(name) },
            .as.functioncall.args = std::move(arguments),
            .parent_function = compiling_function,
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
            .parent_function = compiling_function,
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
            .parent_function = compiling_function,
        };
    }
    
    Expression* make_unaryop(Expression* center, UnaryOperator op) noexcept
    {
        return new Expression
        {
            .type = ExpressionType::UNARYOP,
            .as.unaryop.type = op,
            .as.unaryop.center = center,
            .parent_function = compiling_function,
        };
    }
}
