#include "Expression.hpp"

#include "Context.hpp"
#include "Function.hpp"
#include "Error.hpp"
#include "Driver.hpp"

#include <stdexcept>
#include <exception>

void ti::expr::Numconst::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
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






/*-----------------------------*
 | UNARY EXPRESSION GENERATION |
 *-----------------------------*/

void ti::expr::unary::PlusPlus::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for increment unary expression");
    
    context.add_to_code(ti::format("adc %s, #1\n", ti::location_to_string(allocation.location).c_str()));
}

void ti::expr::unary::MinusMinus::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for decrement unary expression");
}

void ti::expr::unary::Addrof::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for address-of unary expression");
}

void ti::expr::unary::Deref::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for dereference unary expression");
}

void ti::expr::unary::Positive::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for nop (+) unary expression");
}

void ti::expr::unary::Negative::generate(ti::Context& context, ti::Function& function, const ti::ForcedAllocation& allocation) noexcept
{
    ti::write_log("Generating code for negate unary expression");
}
