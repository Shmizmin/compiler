#include "Expression.hpp"

#include "Context.hpp"
#include "Function.hpp"
#include "Error.hpp"
#include "Central.hpp"
#include "IR.hpp"

#include <iostream>
#include <stdexcept>
#include <exception>

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
    
    int generate_uuid(void) noexcept
    {
        static int uuid = 0;
        
        return uuid++;
    }
    
}
    
namespace
{
#define symbols common.context.symbol_table
#define common2 { common.context, common.parent_function, new_allocation.allocation }
    void compile_numconst(const ti::expr::Numconst& numconst, ti::CommonArgs& common) noexcept
    {
        ti::write_log("\t\tCompiling 8-bit numeric constant expression");
        
        common.context.emit_ldb(common.allocation.location, numconst.value);
    }
    
    void compile_stringconst(const ti::expr::Stringconst& stringconst, ti::CommonArgs& common) noexcept
    {
        ti::write_log("\t\tCompiling string constant expression");
        
        const auto ptr = common.context.allocate_heap(stringconst.text.size());
        
        // .org ptr
        common.context.emit_org(ptr);
        /*common.context.add_command(ti::Directive
        {
            .type = ti::DirectiveType::ORIGIN,
            .as.origin.address = ptr,
        });*/
        
        // .ascii "text"
        common.context.emit_ascii(stringconst.text);
        /*common.context.add_command(ti::Directive
        {
            .type = ti::DirectiveType::ASCII,
            .as.ascii.text = stringconst.text,
        });*/
        
        
        // push rx
        common.context.emit_push(ptr & 0x00FF);
        /*common.context.add_command(ti::Instruction
        {
            .type = ti::InstructionType::STACK,
            .as.stack.dir = ti::insn::Stack::Direction::PUSH,
            .as.stack.data = (ptr & 0x00FF),
        });*/
        
        // push rx
        common.context.emit_push(ptr & 0xFF00);
        /*common.context.add_command(ti::Instruction
        {
            .type = ti::InstructionType::STACK,
            .as.stack.dir = ti::insn::Stack::Direction::PUSH,
            .as.stack.data = (ptr & 0xFF00),
        });*/
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
                common.context.emit_ldb(common.allocation.location, symbol. as.variable.address);
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
                        // TODO: verify that we dont need to check other fields of the 'Symbol' struct
                        if (argument_symbol->name == ti::format("%s_%s", function_call.left.name, symbol->as.function.arguments[i].name))
                        {
                            argument_symbol->defined = true;
                            
                            ti::compile_expression(symbol.as.function.arguments[i], common);
                            
                            common.context.emit_stb(argument_symbol.as.variable.address, common.allocation.location);
                        }
                    }
                }
                
                //first arg is call_rx (return location of function)
                common.context.emit_call(allocation.location, ti::format("function_start_%s", symbol.name));
            }
        }
    }
    
    void compile_ternaryop(const ti::expr::Ternaryop& ternaryop, ti::CommonArgs& common) noexcept
    {
        ti::write_log("\t\tCompiling ternary expresssion");
                    
        ti::compile_expression(ternaryop.left, common);
                    
        //verify that we can perform a conditional jump on the expression with valid flags
        //assume it is a register for now
        common.context.emit_updateflags(common.allocation.location);
                   
        const auto label_template = "ternary_%s_%s_";
                    
        const auto  true_label = ti::format(label_template, common.parent_function.name, "true"),
                   false_label = ti::format(label_template, common.parent_function.name, "false"),
                     end_label = ti::format(label_template, common.parent_function.name, "end");
        
         true_label.append(::generate_uuid());
        false_label.append(::generate_uuid());
          end_label.append(::generate_uuid());
                    
        common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, false_label);
                    
        common.context.emit_label(true_label);
        ti::generate_expression(ternaryop.center, common);
                    
        common.context.emit_lor(common.allocation.location, 1);
        common.context.emit_jmp(ti::insn::Jmp::Condtion::JEZ, end_label);
        
        common.context.emit_label(false_label);
        ti::generate_expression(ternaryop.right, common);
        
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
                    for (const auto& symbol : symbols)
                    {
                        if (symbol.type == ti::SymbolType::VARIABLE &&
                            symbol.name == binaryop.left->as.identifier.name)
                        {
                            if (symbol.as.variable.visibility == ti::TypeVisibility::LOCAL &&
                                !funcs_equal(symbol.as.variable.parent_function, common.parent_function))
                            {
                                //throw error maybe?
                            }
                            
                            ti::compile_expression(binaryop.right, common);
                            common.context.emit_stb(symbol.as.variable.address, common.allocation.location);
                        }
                    }
                }
            } break;
                
            case PLUS:
            {
                ti::write_log("\t\t\tCompiling addition binary operator");
                
                ti::compile_expression(binaryop.left, common);
                
                ti::Allocator new_allocation
                {
                    ti::AllocatorType::ALLOCATE_FORCED_EXCLUDE,
                    common.allocation,
                };
                
                ti::compile_expression(binaryop.right, common2);
                common.context.emit_adc(common.allocation.location, new_allocation.location());
                
            } break;
                
            case MINUS:
            {
                ti::write_log("\t\t\tCompiling subtraction binary operator");
                
                ti::Allocator new_allocation
                {
                    ti::AllocatorType::ALLOCATE_FORCED_EXCLUDE,
                    common.allocation,
                };
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, common2);
                
                common.context.emit_sbb(common.allocation.location, new_allocation.location());
            } break;
                
            case LEFT_SHIFT:
            {
                ti::write_log("\t\t\tCompiling left shift binary operator");
                
                //once constant expressions are implemented, more than just numbers can be on right hand side
                //ie [5 + 1 << 3] may be considered a valid rvalue
                if (binaryop.right->type != ti::ExpressionType::NUMCONST)
                {
                    ti::compile_expression(binaryop.left);
                    common.context.emit_rol(allocation.location, binaryop.right->as.numconst.value);
                }
            } break;
                
            case RIGHT_SHIFT:
            {
                ti::write_log("\t\t\tCompiling right shift binary operator");
                
                //see left shift for later improvements
                if (binaryop.right->type != ti::ExpressionType::NUMCONST)
                {
                    ti::compile_expression(binaryop.left);
                    common.context.emit_ror(common.allocation.location, binaryop.right->as.numconst.value);
                }
            } break;
                
            case BIT_XOR:
            {
                ti::write_log("\t\t\tCompiling bitwise xor binary expression");
                
                ti::Allocator new_allocation
                {
                    ti::AllocatorType::ALLOCATE_FORCED_EXCLUDE,
                    common.allocation,
                };
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, common2);
                
                common.context.emit_xor(common.allocation.location, new_allocation.location());
            } break;
                
            case BIT_AND:
            {
                ti::write_log("\t\t\tCompiling bitwise and binary expression");
                
                ti::Allocator new_allocation
                {
                    ti::AllocatorType::ALLOCATE_FORCED_EXCLUDE,
                    common.allocation,
                };
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, common2);
                    
                common.context.emit_and(common.allocation.location, new_allocation.location());
            } break;
                
            case BIT_OR:
            {
                ti::write_log("\t\t\tCompiling bitwise or binary expression");
                
                ti::Allocator new_allocation
                {
                    ti::AllocatorType::ALLOCATE_FORCED_EXCLUDE,
                    common.allocation,
                };
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, common2);
                
                common.context.emit_lor(common.allocation.location, new_allocation.location());
            } break;
                
            case EQUALS_EQUALS:
            {
                ti::write_log("\t\t\tCompiling equality test binary expression");
                
                ti::Allocator new_allocation
                {
                    ti::AllocatorType::ALLOCATE_FORCED_EXCLUDE,
                    common.allocation,
                };
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, common2);
                
                // FIXME: maybe? zero flag will be set if equal
                common.context.emit_sbb(common.allocation.location, new_allocation.location());
            } break;
                
            case NOT_EQUALS:
            {
                ti::write_log("\t\t\tCompiling non-equality test binary expression");
                
                ti::Allocator new_allocation
                {
                    ti::AllocatorType::ALLOCATE_FORCED_EXCLUDE,
                    common.allocation,
                };
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, common);
                
                const auto label_template = "%s_%s_%u";
                
                const auto oneify_label = ti::format(label_template, "oneify", common.parent_function.name, ::generate_uuid()),
                              end_label = ti::format(label_template, "end",    common.parent_function.name, ::generate_uuid());
                
                // FIXME: maybe? zero flag will be set if not equal
                common.context.emit_sbb(common.allocation.location, new_allocation.location());
                
                common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, oneify_label);
                common.context.emit_and(common.allocation.location, 0);
                common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, end_label);
                common.context.emit_label(oneify_label);
                common.context.emit_or(common.allocation.location, 1);
                common.context.emit_label(end_label);
            } break;
                
            case LESS:
            {
                ti::write_log("\t\t\tCompiling less-than binary expression");
                
                ti::Allocator new_allocation
                {
                    ti::AllocatorType::ALLOCATE_FORCED_EXCLUDE,
                    common.allocation,
                };
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, common);
                
                const auto label_template = "%s_%s_%u";
                
                const auto less_label = ti::format(label_template, "less", common.parent_function.name, ::generate_uuid()),
                            end_label = ti::format(label_template, "end",  common.parent_function.name, ::generate_uuid());
                
                common.context.emit_sbb(common.allocation.location, new_allocation.location());
                
                //if carry is set, first operand is less
                //if carry not set, first operand is greater or equal
                
                common.context.emit_jmp(ti::insn::Jmp::Condition::JCS, less_label);
                common.context.emit_and(common.allocation.location, 0);
                common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, end_label);
                common.context.emit_label(less_label);
                common.context.emit_or(common.allocation.location, 1);
                common.context.emit_label(end_label);
            } break;
                
            case GREATER:
            {
                ti::write_log("\t\t\tCompiling greater-than binary expression");
                
                ti::Allocator new_allocation
                {
                    ti::AllocatorType::ALLOCATE_FORCED_EXCLUDE,
                    common.allocation,
                };
                
                ti::compile_expression(binaryop.left, common);
                ti::compile_expression(binaryop.right, common);
                
                const auto label_template = "%s_%s_%u";
                
                const auto less_label = ti::format(label_template, "greater", common.parent_function.name, ::generate_uuid()),
                            end_label = ti::format(label_template, "end",  common.parent_function.name, ::generate_uuid());
                
                common.context.emit_sbb(common.allocation.location, new_allocation.location());
                
                //if carry is set, first operand is less
                //if carry not set, first operand is greater or equal
                
                common.context.emit_jmp(ti::insn::Jmp::Condition::JCS, less_label);
                common.context.emit_or(common.allocation.location, 1);
                common.context.emit_jmp(ti::insn::Jmp::Condition::JEZ, end_label);
                common.context.emit_label(less_label);
                common.context.emit_and(common.allocation.location, 0);
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
                common.context.emit_adc(allocation.location, 1);
            } break;
                
            case MINUS_MINUS:
            {
                ti::write_log("\t\t\tCompiling decrement unary operator");
                
                ti::compile_expression(unaryop.center, common);
                common.context.emit_sbb(allocation.location, 1);
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
                
                common.context.emit_not(common.allocation.location);
                common.context.emit_adc(common.allocation.location, 1);
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



/*
void ti::expr::Stringconst::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for string constant expression");
    
    const auto ptr = context.allocate_heap(value.size());
    
    context.add_to_end(ti::format(".org %u", ptr));
    context.add_to_end(ti::format(".ascii \"%s\"", value.c_str()));
    
    context.add_to_code(ti::format("\tpush #%u\n", ptr & 0x00FF));
    context.add_to_code(ti::format("\tpush #%u\n", ptr & 0xFF00));
}

void ti::expr::Identifier::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for identifier expression");
    
    const auto loc = std::find_if(context.symbol_table.begin(), context.symbol_table.end(), [&](Symbol* s)
    {
        auto names_eq = (name == s->name);
        auto quals_eq = false;
        
        if (s->type == ti::SymbolType::VARIABLE)
        {
            auto ns = static_cast<ti::VariableSymbol*>(s);
            
            auto qual = ti::format("%s_%s", ns->function.name.c_str(), name.c_str());
            std::cout << qual << std::endl;
            
            quals_eq = (s->name == qual);
        }
        
        return (names_eq || quals_eq);
    });
    
    const auto loc2 = std::find_if(function.arguments.begin(), function.arguments.end(), [&](Argument& a)
    {
        return (name == a.name);
    });
    
    
    if (loc2 != std::end(function.arguments))
    {
        auto sym = ti::format("%s_%s", function.name.c_str(), name.c_str());
        
        const auto loc3 = std::find_if(context.symbol_table.begin(), context.symbol_table.end(), [&](Symbol* s)
        {
            return (s->type == ti::SymbolType::VARIABLE &&
                    s->name == sym);
        });
        
        if (loc3 == std::end(context.symbol_table))
        {
            ti::throw_error("Parameter %s is previously undefined correctly", name.c_str());
        }
        
        if ((*loc3)->type != ti::SymbolType::VARIABLE)
        {
            ti::throw_error("Parameter %s is previously defined not as a variable", name.c_str());
        }
        auto var = static_cast<ti::VariableSymbol*>(*loc3);
        
        if (!::funcs_equal(var->function, function))
        {
            ti::throw_error("Parameter %s is defined for a different function", name.c_str());
        }
        
        context.add_to_code(ti::format("\tldb %s, %%%u\n", ti::location_to_string(allocation.location).c_str(), var->address));
        
        return;
    }
    
    
    if (loc == std::end(context.symbol_table))
    {
        ti::throw_error("Identifier %s is previously undefined", name.c_str());
    }
    if (!((*loc)->defined))
    {
        ti::throw_error("Identifier %s was not previously assigned a valid value", name.c_str());
    }
    if ((*loc)->type == ti::SymbolType::FUNCTION)
    {
        ti::throw_error("Identifier %s was previously defined as a function", name.c_str());
    }
    else
    {
        //symbol seems valid and of type variable
        auto nsym = static_cast<ti::VariableSymbol*>(*loc);
        
        if (nsym->visibility == ti::TypeVisibility::LOCAL && !::funcs_equal(nsym->function, function))
        {
            //variable is local to a different function
#warning this
            //ti::throw_error("Identifier %s is inaccessible from function %s", name.c_str(), function.name.c_str());
        }
        
        //variable is good to query and load
        context.add_to_code(ti::format("\tldb %s, %%%u\n", ti::location_to_string(allocation.location).c_str(), nsym->address));
    }
}

void ti::expr::Ternary::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for ternary expression");
    
    left->generate(context, function, allocation);
    
    context.add_to_code("\tupdateFlags()\n");
    
    const auto label_template = "ternary_%s_%s_%u";
    

    //go to correct condition label
    const auto second_label = ti::format(label_template, "false", function.name.c_str(), context.counter);
    context.add_to_code(ti::format("\tjez %s\n", second_label.c_str()).c_str());
    
    const auto end_label = ti::format(label_template, "end", function.name.c_str(), context.counter);
    
    //true label
    context.add_to_code(ti::format("@%s:\n", ti::format(label_template, "true", function.name.c_str(), context.counter).c_str()));
    center->generate(context, function, allocation);
    context.add_to_code(ti::format("\tjmp(%s)\n", end_label.c_str()).c_str());
    
    //false label
    context.add_to_code(ti::format("@%s:\n", second_label.c_str()));
    right->generate(context, function, allocation);
    
    //end label
    context.add_to_code(ti::format("@%s:\n", end_label.c_str()).c_str());
}





void ti::expr::Numconst::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for 8-bit numeric constant expression");
    
    context.add_to_code(ti::format("\tldb %s, #%u\n", ti::location_to_string(allocation.location).c_str(), value));
}




void ti::expr::FCall::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for function call expression");
    
    auto* l = static_cast<ti::expr::Identifier*>(this->left);
    
    const auto loc = std::find_if(context.symbol_table.begin(), context.symbol_table.end(), [&](Symbol* s)
    {
        return (l->name == s->name);
    });
    
    if (loc == std::end(context.symbol_table))
    {
        ti::throw_error("Identifier %s is previously undefined", l->name.c_str());
    }
    
    const auto res = *loc;
    
    if (res->type != ti::SymbolType::FUNCTION)
    {
        ti::throw_error("Identifier %s was previously defined as a variable", l->name.c_str());
    }
    if (!res->defined)
    {
        ti::throw_error("Identifier %s was not previously defined with a valid function body", l->name.c_str());
    }
    else
    {
        auto* nsym = static_cast<ti::FunctionSymbol*>(res);
    
        
        const auto def_num = nsym->arguments.size();
        const auto fcl_num = args.size();
        
        if (def_num != fcl_num)
        {
            //arity of definition and call do not match
            ti::throw_error("Function %s takes %u arguments but was called with %u", l->name.c_str(), def_num, fcl_num);
        }
        
        
        for (auto i = 0; i < fcl_num; ++i)
        {
            auto farg = std::find_if(context.symbol_table.begin(), context.symbol_table.end(), [&](Symbol* s)
            {
                std::string a = ti::format("%s_%s", l->name.c_str(), nsym->arguments[i].name.c_str());
                std::cout << a << std::endl;
                return (s->name == a);
            });
            
            if (farg == std::end(context.symbol_table))
            {
                ti::throw_error("Cannot bind to argument %s", nsym->arguments[i].name.c_str());
            }
            
            auto fsym = static_cast<ti::VariableSymbol*>(*farg);
            
            fsym->defined = true;
            args[i]->generate(context, function, allocation);
            
            context.add_to_code(ti::format("\tstb %%%u, %s\n", fsym->address, ti::location_to_string(allocation.location).c_str()));
        }
        
        context.add_to_code(ti::format("\tcall_%s(function_start_%s)\n", ti::location_to_string(allocation.location).c_str(), nsym->name.c_str()));
    }
}


void ti::expr::binary::Equals::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary assignment expression");
    
    if (left->type != ti::ExpressionType::IDENTIFIER)
    {
        ti::throw_error("Left side of assignment is not an identifier");
    }
    
    auto* ident = static_cast<ti::expr::Identifier*>(left);
    
    const auto loc = std::find_if(context.symbol_table.begin(), context.symbol_table.end(), [&](Symbol* s)
    {
        return (ident->name == s->name);
    });
    
    
    if (loc == std::end(context.symbol_table))
    {
        ti::throw_error("Identifier %s is previously undefined", ident->name.c_str());
    }
    if (!((*loc)->defined))
    {
        ti::throw_error("Identifier %s was not previously assigned a valid value", ident->name.c_str());
    }
    if ((*loc)->type == ti::SymbolType::FUNCTION)
    {
        ti::throw_error("Identifier %s was previously defined as a function", ident->name.c_str());
    }
    else
    {
        //symbol seems valid and of type variable
        auto nsym = static_cast<ti::VariableSymbol*>(*loc);
        
        if (nsym->visibility == ti::TypeVisibility::LOCAL && !::funcs_equal(nsym->function, function))
        {
            //variable is local to a different function
            ti::throw_error("Identifier %s is inaccessible from function %s", ident->name.c_str(), function.name.c_str());
        }
        
        right->generate(context, function, allocation);
        context.add_to_code(ti::format("\tstb %%%u, %s\n", nsym->address, ti::location_to_string(allocation.location).c_str()).c_str());
    }
}

void ti::expr::binary::Plus::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary addition expression");
    
    left->generate(context, function, allocation);
    
    const auto alloc = context.allocate_forced(allocation);
    right->generate(context, function, alloc);
    
    context.add_to_code(ti::format("\tadc %s, %s\n", ti::location_to_string(allocation.location).c_str(), ti::location_to_string(alloc.location).c_str()).c_str());
    
    context.deallocate_forced(alloc);
}

void ti::expr::binary::Minus::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary subtraction expression");
    
    left->generate(context, function, allocation);
    
    const auto alloc = context.allocate_forced(allocation);
    right->generate(context, function, alloc);
    
    context.add_to_code(ti::format("\tsbb %s, %s\n", ti::location_to_string(allocation.location).c_str(), ti::location_to_string(alloc.location).c_str()).c_str());
    
    context.deallocate_forced(alloc);
}


void ti::expr::binary::LeftShift::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary left shift expression");
    
    left->generate(context, function, allocation);
    
    //optimize by adding constant expressions to right hand side which can be more than just a number, ie [5 + 1 << 3]
    if (right->type != ti::ExpressionType::NUMCONST)
    {
        ti::throw_error("Right hand side of a bit shift expression must be a constant expression");
    }
    
    auto* nsym = static_cast<ti::expr::Numconst*>(right);
    
    context.add_to_code(ti::format("\trol %s, #%u\n", ti::location_to_string(allocation.location).c_str(), nsym->value).c_str());
}

void ti::expr::binary::RightShift::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary right shift expression");
    
    left->generate(context, function, allocation);
    
    //optimize by adding constant expressions to right hand side which can be more than just a number, ie [5 + 1 << 3]
    if (right->type != ti::ExpressionType::NUMCONST)
    {
        ti::throw_error("Right hand side of a bit shift expression must be a constant expression");
    }
    
    auto* nsym = static_cast<ti::expr::Numconst*>(right);
    
    context.add_to_code(ti::format("\tror %s, #%u\n", ti::location_to_string(allocation.location).c_str(), nsym->value).c_str());
}


void ti::expr::binary::BitXor::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary bitwise xor expression");
    
    left->generate(context, function, allocation);
    
    const auto alloc = context.allocate_forced(allocation);
    right->generate(context, function, alloc);
    
    //could add ast optimizations to be able to use xorImm() instead
    context.add_to_code(ti::format("\txorR(%s, %s)\n", ti::location_to_string(allocation.location).c_str(), ti::location_to_string(alloc.location).c_str()).c_str());
    
    context.deallocate_forced(alloc);
    
}

void ti::expr::binary::BitAnd::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary bitwise and expression");
    
    left->generate(context, function, allocation);
    
    const auto alloc = context.allocate_forced(allocation);
    right->generate(context, function, alloc);
    
    context.add_to_code(ti::format("\tand %s, %s\n", ti::location_to_string(allocation.location).c_str(), ti::location_to_string(alloc.location).c_str()).c_str());
    
    context.deallocate_forced(alloc);
}

void ti::expr::binary::BitOr::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary bitwise or expression");
    
    left->generate(context, function, allocation);
    
    const auto alloc = context.allocate_forced(allocation);
    right->generate(context, function, alloc);
    
    context.add_to_code(ti::format("\tor %s, %s\n", ti::location_to_string(allocation.location).c_str(), ti::location_to_string(alloc.location).c_str()).c_str());
    
    context.deallocate_forced(alloc);
}


void ti::expr::binary::EqualsEquals::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary equals-equals expression");
    
    left->generate(context, function, allocation);
    
    const auto alloc = context.allocate_forced(allocation);
    right->generate(context, function, alloc);
    
    //zero flag will be set if equal
    context.add_to_code(ti::format("\tsbb %s, %s\n", ti::location_to_string(allocation.location).c_str(), ti::location_to_string(alloc.location).c_str()).c_str());
}

void ti::expr::binary::NotEquals::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary not-equals expression");
    
    left->generate(context, function, allocation);
    
    const auto alloc = context.allocate_forced(allocation);
    right->generate(context, function, alloc);
    
    //zero flag will be set if not equal
    const auto loc1 = ti::location_to_string(allocation.location);
    const auto loc2 = ti::location_to_string(alloc.location);
    const auto zero_template = ti::format("zero_%s_%u", function.name.c_str(), context.counter);
    
    context.add_to_code(ti::format("\tsbb %s, %s\n", loc1.c_str(), loc2.c_str()));
    context.add_to_code(ti::format("\tjez %s\n", zero_template.c_str()));
    context.add_to_code(ti::format("\tldb %s, #0\n", loc1.c_str()));
    
    const auto end_template = ti::format("end_%s_%u", function.name.c_str(), context.counter);
    context.add_to_code(ti::format("\tjmp(%s)\n", end_template.c_str()));
    
    context.add_to_code(ti::format("@%s:\n", zero_template.c_str()).c_str());
    
    context.add_to_code(ti::format("\tldb %s, #1\n", loc1.c_str()));
    
    context.add_to_code(ti::format("@%s:\n", end_template.c_str()).c_str());
    
}

void ti::expr::binary::Less::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary less expression");
    
    left->generate(context, function, allocation);
    
    const auto alloc = context.allocate_forced(allocation);
    right->generate(context, function, alloc);
    
    const auto loc1 = ti::location_to_string(allocation.location);
    const auto loc2 = ti::location_to_string(alloc.location);
    const auto less_template = ti::format("less_%s_%u", function.name.c_str(), context.counter);
    
    context.add_to_code(ti::format("\tsbb %s, %s\n", loc1.c_str(), loc2.c_str()));
    context.add_to_code(ti::format("\tjcc(%s)\n", less_template.c_str()));
    context.add_to_code(ti::format("\tldb %s, #0\n", loc1.c_str()));
    
    const auto end_template = ti::format("end_%s_%u", function.name.c_str(), context.counter);
    context.add_to_code(ti::format("\tjmp(%s)\n", end_template.c_str()));
    
    context.add_to_code(ti::format("@%s:\n", less_template.c_str()).c_str());
    
    context.add_to_code(ti::format("\tldb %s, #1\n", loc1.c_str()));
    
    context.add_to_code(ti::format("@%s:\n", end_template.c_str()).c_str());
    
}

void ti::expr::binary::Greater::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary greater expression");
    
    left->generate(context, function, allocation);
    
    const auto alloc = context.allocate_forced(allocation);
    right->generate(context, function, alloc);
    
    const auto loc1 = ti::location_to_string(allocation.location);
    const auto loc2 = ti::location_to_string(alloc.location);
    const auto greater_template = ti::format("greater_%s_%u", function.name.c_str(), context.counter);
    
    context.add_to_code(ti::format("\tsbb %s, %s\n", loc1.c_str(), loc2.c_str()));
    context.add_to_code(ti::format("\tjcc(%s)\n", greater_template.c_str()));
    context.add_to_code(ti::format("\tldb %s, #0\n", loc1.c_str()));
    
    const auto end_template = ti::format("end_%s_%u", function.name.c_str(), context.counter);
    context.add_to_code(ti::format("\tjmp(%s)\n", end_template.c_str()));
    
    context.add_to_code(ti::format("@%s:\n", greater_template.c_str()).c_str());
    
    context.add_to_code(ti::format("\tldb %s, #1\n", loc1.c_str()));
    
    context.add_to_code(ti::format("@%s:\n", end_template.c_str()).c_str());
}




void ti::expr::unary::PlusPlus::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for increment unary expression");
    
    center->generate(context, function, allocation);
    
    context.add_to_code(ti::format("\tadc %s, #1\n", ti::location_to_string(allocation.location).c_str()));
}

void ti::expr::unary::MinusMinus::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for decrement unary expression");
    
    center->generate(context, function, allocation);
    
    context.add_to_code(ti::format("\tsbb %s, #1\n", ti::location_to_string(allocation.location).c_str()));
}

void ti::expr::unary::Positive::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for positive unary expression");
    
    center->generate(context, function, allocation);
}

void ti::expr::unary::Negative::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for negate unary expression");
    
    center->generate(context, function, allocation);
    
    const auto loc = ti::location_to_string(allocation.location);
    
    context.add_to_code(ti::format("\tnot %s\n", loc.c_str()));
    context.add_to_code(ti::format("\tadc %s, #1\n", loc.c_str()));
}
*/
