#include "Expression.hpp"

#include "Context.hpp"
#include "Function.hpp"
#include "Error.hpp"
#include "Driver.hpp"

#include <stdexcept>
#include <exception>

namespace
{
    bool funcs_equal(ti::Function& f1, ti::Function& f2)
    {
        bool args_same = f1.arguments.size() == f2.arguments.size();
        if (args_same)
        {
            for (auto i = 0; i < f1.arguments.size(); ++i)
            {
                const auto& arg1 = f1.arguments[i];
                const auto& arg2 = f2.arguments[i];
                
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
               (f1.return_type.qualifier == f2.return_type.qualifier) &&
                args_same;
    }
}


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
        return (name == s->name);
    });
    
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
            ti::throw_error("Identifier %s is inaccessible from function %s", name.c_str(), function.name.c_str());
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



/*--------------------------------*
 | NUMCONST EXPRESSION GENERATION |
 *--------------------------------*/

void ti::expr::Numconst::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for 8-bit numeric constant expression");
    
    context.add_to_code(ti::format("\tldb %s, #%u\n", ti::location_to_string(allocation.location).c_str(), value));
}



/*------------------------------*
 | BINARY EXPRESSION GENERATION |
 *------------------------------*/

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
        
        /*
         TypeVisibility visibility;
         CompleteType type;
         std::string name;
         Expression* value;
         */
        
        const auto def_num = nsym->arguments.size();
        const auto fcl_num = args.size();
        
        if (def_num != fcl_num)
        {
            //arity of definition and call do not match
            ti::throw_error("Function %s takes %u arguments but was called with %u", l->name.c_str(), def_num, fcl_num);
        }
        
        auto* var_stmt = new ti::stmt::Variable();
        var_stmt->type = ti::StatementType::VARIABLE;
        var_stmt->variables = {};
        
        for (auto i = 0; i < fcl_num; ++i)
        {
            const auto& arg = nsym->arguments[i];
            
            //will leak
            auto* var = new ti::Variable();

            context.counter++;
            var->name = ti::format("%s_%s_%u", function.name.c_str(), arg.name.c_str(), context.counter);
            var->type = arg.type;
            var->visibility = ti::TypeVisibility::LOCAL;
            var->value = args[i];
            
            var_stmt->variables.emplace_back(var);
        
        }
        
        var_stmt->generate(context, function);
        
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



/*-----------------------------*
 | UNARY EXPRESSION GENERATION |
 *-----------------------------*/

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
