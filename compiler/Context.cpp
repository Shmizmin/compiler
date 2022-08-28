#include "Context.hpp"
#include "Error.hpp"
#include "Driver.hpp"

#include <algorithm>


void ti::Context::add_to_symbol_table(ti::Symbol* symbol) noexcept
{
    if (std::find_if(symbol_table.begin(), symbol_table.end(), [&](ti::Symbol* s)
        {
            return (s->name == symbol->name);
        }) != std::end(symbol_table))
    {
        //duplicate symbol name
        ti::throw_error("Duplicate identifier %s", symbol->name.c_str());
    }
    
    symbol_table.emplace_back(symbol);
}


void ti::Context::add_to_code(const std::string& code) noexcept
{
    counter++;
    code_segment.append(code);
}

void ti::Context::add_to_end(const std::string& code) noexcept
{
    end_segment.append(code);
}

const ti::ForcedAllocation ti::Context::allocate_forced(void) noexcept
{
    auto alloc = allocate();
    auto was_forced = false;
    
    if (alloc == STACK)
    {
        //no free registers
        add_to_code("\tpush r0\n");
        alloc = R0;
        was_forced = true;
    }
    
    return { alloc, was_forced };
}

//parameter allocation is location to exclude allocating
const ti::ForcedAllocation ti::Context::allocate_forced(const ti::ForcedAllocation& allocation) noexcept
{
    auto alloc = allocate();
    auto was_forced = false;
    
    if (alloc == STACK)
    {
        switch (allocation.location)
        {
            case R0: alloc = R1; break;
            case R1: alloc = R2; break;
            case R2: alloc = R3; break;
            case R3: alloc = R0; break;
            default: ti::throw_error("Allocation exclusion had an illegal allocation location"); break;
        }
        
        add_to_code(ti::format("\tpush %s\n", ti::location_to_string(alloc).c_str()));
        was_forced = true;
    }
    
    return { alloc, was_forced };
}

void ti::Context::deallocate_forced(const ti::ForcedAllocation& alloc) noexcept
{
    if (alloc.was_forced)
    {
        add_to_code(ti::format("\tpop %s\n", ti::location_to_string(alloc.location).c_str()));
    }
    else
    {
        deallocate(alloc.location);
    }
}

const ti::Location ti::Context::allocate(void) noexcept
{
    if (available_registers[0])
    {
        available_registers[0] = false;
        return R0;
    }
    else if (available_registers[1])
    {
        available_registers[1] = false;
        return R1;
    }
    else if (available_registers[2])
    {
        available_registers[2] = false;
        return R2;
    }
    else if (available_registers[3])
    {
        available_registers[3] = false;
        return R3;
    }
    else // stack should always be available
        return STACK;
}

void ti::Context::deallocate(const ti::Location& location) noexcept
{
    switch (location)
    {
        case R0: available_registers[0] = true; break;
        case R1: available_registers[1] = true; break;
        case R2: available_registers[2] = true; break;
        case R3: available_registers[3] = true; break;
        default: ti::throw_error("Illegal deallocation location"); break;
    }
}

std::uint16_t ti::Context::allocate_heap(std::uint16_t bytes) noexcept
{
    const auto size = bytes;
    const auto length = available_heap.size();
    
    std::uint16_t cvt = 0;
    auto avail = true;
    for (auto i = 0; i < length; ++i)
    {
        //potential heap address
        cvt = static_cast<std::uint16_t>(i);
        
        for (auto j = 0; j < size; ++j)
        {
            if (!available_heap[j])
            {
                avail = false;
                continue;
            }
        }
    }
    
    if (avail)
    {
        return cvt;
    }
    else
    {
        //no successful allocation was made
        ti::throw_error("No contiguous heap region large enough allocate %u bytes", size);
    }
}

void ti::Context::deallocate_heap(std::uint16_t location, std::uint16_t bytes) noexcept
{
    if (location < 0x4000 && location < available_heap.size())
    {
        for (auto i = location; i < location + bytes; ++i)
        {
            available_heap[i] = true;
        }
    }
    else
    {
        ti::throw_error("Address %u is outside the heap region and cannot be deallocated", location);
    }
}


std::uint8_t ti::get_type_size(ti::CompleteType& type) noexcept
{
    using enum ti::TypeQualifier;
    using enum ti::TypeSpecifier;
    
    if (type.qualifier == PTR)
    {
        return 2;
    }
    else
    {
        switch (type.specifier)
        {
            case BYTE: return 1; break;
            case VOID: ti::throw_error("Type 'void' is unsized");            break;
            default:   ti::throw_error("Unknown type to query the size of"); break;
        }
    }
}

std::string ti::location_to_string(ti::Location location) noexcept
{
    switch (location)
    {
        case R0:    return "r0";    break;
        case R1:    return "r1";    break;
        case R2:    return "r2";    break;
        case R3:    return "r3";    break;
        case STACK: return "stack"; break;
        default: ti::throw_error("Unknown location to convert to string"); break;
    }
}

std::string ti::expression_type_to_string(ti::ExpressionType type) noexcept
{
    using enum ti::ExpressionType;
    switch (type)
    {
        case NUMCONST:    return "numeric constant"; break;
        case STRINGCONST: return "string constant";  break;
        case IDENTIFIER:  return "identifier";       break;
        case TERNARYOP:   return "ternary operator"; break;
        case BINARYOP:    return "binary operator";  break;
        case UNARYOP:     return "unary operator";   break;
        default: ti::throw_error("Unknown expresion type to convert to string"); break;
    }
}
