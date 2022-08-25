#include "Expression.hpp"

#include "Context.hpp"
#include "Function.hpp"
#include "Error.hpp"
#include "Driver.hpp"

#include <stdexcept>
#include <exception>


/*--------------------------------*
 | NUMCONST EXPRESSION GENERATION |
 *--------------------------------*/

void ti::expr::num::Numconst8::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    
}

void ti::expr::num::Numconst16::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    
}




void ti::expr::Stringconst::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    
}

void ti::expr::Identifier::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    
}

void ti::expr::Ternary::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    
}

void ti::expr::Binary::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    
}


/*------------------------------*
 | BINARY EXPRESSION GENERATION |
 *------------------------------*/





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
