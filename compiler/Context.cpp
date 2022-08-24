#include "Context.hpp"
#include "Error.hpp"

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
    code_segment.append(code);
}

const ti::ForcedAllocation ti::Context::allocate_forced(void) noexcept
{
    auto alloc = allocate();
    auto was_forced = false;
    
    if (alloc == STACK)
    {
        //no free registers
        add_to_code("push r0\n");
        alloc = R0;
        was_forced = true;
    }
    
    return { alloc, was_forced };
}

void ti::Context::deallocate_forced(const ti::ForcedAllocation& alloc) noexcept
{
    if (alloc.was_forced)
    {
        add_to_code("pop r0\n");
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

std::uint16_t ti::Context::allocate_heap(ti::CompleteType& type) noexcept
{
    const auto size = ti::get_type_size(type);
    const auto length = available_heap.size();
    
    for (auto i = 0; i < length; ++i)
    {
        const auto cvt = static_cast<std::uint16_t>(i);
        
        auto& h1 = available_heap[i];
        
        
        switch (size)
        {
            case 1: if (h1)
            {
                h1 = false;
                return cvt;
            } break;
                
            case 2: if (i < length)
            {
                auto& h2 = available_heap[i + 1u];
                
                if (h1 && h2)
                {
                    h1 = false;
                    h2 = false;
                    return cvt;
                }
            } break;
                
            default: ti::throw_error("Heap allocations of size %u are unsupported", size); break;
        }
    }
    
    //no successful allocation was made
    ti::throw_error("No contiguous heap region large enough allocate %u bytes", size);
}

void ti::Context::deallocate_heap(std::uint16_t location, ti::CompleteType& type) noexcept
{
    const auto length = available_heap.size();
    
    if (location < 0x4000 && location < length)
    {
        const auto size = ti::get_type_size(type);
        
        switch (size)
        {
            case 1: available_heap[location] = true; break;
                
            case 2: available_heap[location]     = true;
                    available_heap[location + 1] = true; break;
                
            default: ti::throw_error("Heap deallocations of size %u are unsupported", size); break;
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
            case WORD: return 2; break;
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
