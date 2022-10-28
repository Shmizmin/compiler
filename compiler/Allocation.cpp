#include "Allocation.hpp"
#include "Context.hpp"
#include "Error.hpp"
#include "Types.hpp"

#include <optional>

namespace
{
    std::optional<ti::RegisterType> allocate_register(ti::Context& context) noexcept
    {
        using enum ti::RegisterType;
        if (context.available_registers[0])
        {
            context.available_registers[0] = false;
            return { R0 };
        }
        else if (context.available_registers[1])
        {
            context.available_registers[1] = false;
            return { R1 };
        }
        else if (context.available_registers[2])
        {
            context.available_registers[2] = false;
            return { R2 };
        }
        else if (context.available_registers[3])
        {
            context.available_registers[3] = false;
            return { R3 };
        }
        else
        {
            return std::nullopt;
        }
    }
    
    void deallocate_register(ti::Context& context, ti::RegisterType location) noexcept
    {
        using enum ti::RegisterType;
        switch (location)
        {
            case R0: context.available_registers[0] = true; break;
            case R1: context.available_registers[1] = true; break;
            case R2: context.available_registers[2] = true; break;
            case R3: context.available_registers[3] = true; break;
            default:                                        break;
        }
    }
}

namespace ti
{
    RegisterAllocation::RegisterAllocation(Context& context) noexcept
        : context(context)
    {
        auto new_allocation = ::allocate_register(context);
        
        if (!new_allocation.has_value())
        {
            context.emit_push(RegisterType::R0);
            new_allocation.value() = RegisterType::R0;
            context.available_registers[0] = false;
        }
        
        location = new_allocation.value();
    }
    
    RegisterAllocation::RegisterAllocation(Context& context, RegisterType exclude) noexcept
        : context(context)
    {
        auto new_allocation = ::allocate_register(context);
        
        if (!new_allocation.has_value())
        {
            auto& value = new_allocation.value();
            
            using enum RegisterType;
            switch (value)
            {
                case R0: value = R1; context.available_registers[1] = false; break;
                case R1: value = R2; context.available_registers[2] = false; break;
                case R2: value = R3; context.available_registers[3] = false; break;
                case R3: value = R0; context.available_registers[0] = false; break;
                default:                                                     break;
            }
            
            context.emit_push(value);
        }
        
        location = new_allocation.value();
    }
    
    RegisterAllocation::~RegisterAllocation(void) noexcept
    {
        context.emit_pop(location);
        ::deallocate_register(context, location);
    }
    
    
    HeapAllocation::HeapAllocation(Context& context, std::uint16_t bytes) noexcept
        : context(context), bytes(bytes)
    {
        auto available = true;
        for (std::uint16_t i = 0u; i < context.available_heap.size(); ++i)
        {
            for (auto j = 0u; j < bytes; ++j)
            {
                if (!context.available_heap[i + j])
                    available = false;
            }
            
            if (available)
            {
                for (auto k = 0u; k < bytes; ++k)
                    context.available_heap[i + k] = false;
                
                address = i;
            }
            
            else available = true;
        }
        
        ti::throw_error(fmt::format("No contiguous heap region of {} bytes available", bytes));
    }
    
    HeapAllocation::~HeapAllocation(void) noexcept
    {
        //for (auto i = address; i < address + bytes; ++i)
        //    context.available_heap[i] = true;
    }
}
