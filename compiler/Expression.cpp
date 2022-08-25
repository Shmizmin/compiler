#include "Expression.hpp"

#include "Context.hpp"
#include "Function.hpp"
#include "Error.hpp"
#include "Driver.hpp"

#include <stdexcept>
#include <exception>



void ti::expr::Stringconst::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    
}

void ti::expr::Identifier::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    
}

void ti::expr::Ternary::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    
}



/*--------------------------------*
 | NUMCONST EXPRESSION GENERATION |
 *--------------------------------*/

void ti::expr::num::Numconst8::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for 8-bit numeric constant expression");
}

void ti::expr::num::Numconst16::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for 16-bit numeric constant expression");
}



/*------------------------------*
 | BINARY EXPRESSION GENERATION |
 *------------------------------*/

void ti::expr::binary::Index::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for array index expression");
}

void ti::expr::binary::FCall::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for function call expression");
}

void ti::expr::binary::Equals::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary assignment expression");
}

void ti::expr::binary::Plus::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary addition expression");
}

void ti::expr::binary::Minus::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary subtraction expression");
}

void ti::expr::binary::PlusEquals::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary assignment-addition expression");
}

void ti::expr::binary::MinusEquals::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary assignment-subtraction expression");
}

void ti::expr::binary::LeftShift::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary left shift expression");
}

void ti::expr::binary::RightShift::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary right shift expression");
}

void ti::expr::binary::BitXor::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary bitwise xor expression");
}

void ti::expr::binary::BitAnd::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary bitwise and expression");
}

void ti::expr::binary::BitOr::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary bitwise or expression");
}

void ti::expr::binary::LogAnd::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary logical and expression");
}

void ti::expr::binary::LogOr::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary logical or expression");
}

void ti::expr::binary::EqualsEquals::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary equals-equals expression");
}

void ti::expr::binary::NotEquals::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary not-equals expression");
}

void ti::expr::binary::Less::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary less expression");
}

void ti::expr::binary::LessEquals::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary less-equals expression");
}

void ti::expr::binary::Greater::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary greater expression");
}

void ti::expr::binary::GreaterEquals::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for binary greater-equals expression");
}



/*-----------------------------*
 | UNARY EXPRESSION GENERATION |
 *-----------------------------*/

void ti::expr::unary::PlusPlus::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for increment unary expression");
    
    //return value allocation is free for use
    center->generate(context, function, allocation);
    
    context.add_to_code(ti::format("adc %s, #1\n", ti::location_to_string(allocation.location).c_str()));
}

void ti::expr::unary::MinusMinus::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for decrement unary expression");
    
    //return value allocation is free for use
    center->generate(context, function, allocation);
    
    context.add_to_code(ti::format("sbb %s, #1\n", ti::location_to_string(allocation.location).c_str()));
}

void ti::expr::unary::Addrof::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for address-of unary expression");
    
    ti::throw_error("Unary addressof is unsupported currently");
#warning UNIMPLEMENTED FUNCTION
}

void ti::expr::unary::Deref::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for dereference unary expression");
    
    ti::throw_error("Unary dereferencing is unsupported currently");
#warning UNIMPLEMENTED FUNCTION
}

void ti::expr::unary::Positive::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for nop (+) unary expression");
    
    //return value allocation is free for use
    center->generate(context, function, allocation);
}

void ti::expr::unary::Negative::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for negate unary expression");
    
    //return value allocation is free for use
    center->generate(context, function, allocation);
    
    const auto loc = ti::location_to_string(allocation.location);
    const auto reg = loc.c_str();
    
    context.add_to_code(ti::format("not %s\n", reg));
    context.add_to_code(ti::format("adc %s, #1\n", reg));
}
