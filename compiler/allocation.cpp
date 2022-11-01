#include "allocation.hpp"
#include "compiler.hpp"
#include "error.hpp"
#include "types.hpp"

#include <optional>

namespace
{
    std::optional<ti::RegisterType> allocate_register(ti::Compiler& compiler) noexcept
    {
        using enum ti::RegisterType;
        if (compiler.available_registers[0])
        {
            compiler.available_registers[0] = false;
            return { R0 };
        }
        else if (compiler.available_registers[1])
        {
            compiler.available_registers[1] = false;
            return { R1 };
        }
        else if (compiler.available_registers[2])
        {
            compiler.available_registers[2] = false;
            return { R2 };
        }
        else if (compiler.available_registers[3])
        {
            compiler.available_registers[3] = false;
            return { R3 };
        }
        else
        {
            return std::nullopt;
        }
    }
    
    void deallocate_register(ti::Compiler& compiler, ti::RegisterType location) noexcept
    {
        using enum ti::RegisterType;
        switch (location)
        {
            case R0: compiler.available_registers[0] = true; break;
            case R1: compiler.available_registers[1] = true; break;
            case R2: compiler.available_registers[2] = true; break;
            case R3: compiler.available_registers[3] = true; break;
            default:                                        break;
        }
    }
}


namespace ti
{
    RegisterAllocation::RegisterAllocation(bool was_forced, RegisterType location, Compiler& compiler) noexcept
        : was_forced(was_forced), location(location), compiler(compiler)
    {
    }
    
    RegisterAllocation::RegisterAllocation(Compiler& compiler) noexcept
        : compiler(compiler)
    {
        auto new_allocation = ::allocate_register(compiler);
        
        if (!new_allocation.has_value())
        {
            was_forced = true;
            compiler.emit_push(RegisterAllocation{ true, RegisterType::R0, compiler });
            new_allocation.value() = RegisterType::R0;
            compiler.available_registers[0] = false;
        }
        
        location = new_allocation.value();
    }
    
    RegisterAllocation::RegisterAllocation(Compiler& compiler, const RegisterAllocation& exclude) noexcept
        : compiler(compiler)
    {
        auto new_allocation = ::allocate_register(compiler);
        
        if (!new_allocation.has_value())
        {
            auto& value = new_allocation.value();
            
            using enum RegisterType;
            switch (value)
            {
                case R0: value = R1; compiler.available_registers[1] = false; break;
                case R1: value = R2; compiler.available_registers[2] = false; break;
                case R2: value = R3; compiler.available_registers[3] = false; break;
                case R3: value = R0; compiler.available_registers[0] = false; break;
                default:                                                     break;
            }
            
            compiler.emit_push({ true, value, compiler });
        }
        
        location = new_allocation.value();
    }
    
    RegisterAllocation::~RegisterAllocation(void) noexcept
    {
        if (was_forced)
        {
            compiler.emit_pop(RegisterAllocation{ true, location, compiler });
        }
            
        ::deallocate_register(compiler, location);
    }
    
    
    HeapAllocation::HeapAllocation(Compiler& compiler, std::uint16_t bytes) noexcept
        : compiler(compiler), bytes(bytes)
    {
        auto available = true;
        for (std::uint16_t i = 0u; i < compiler.available_heap.size(); ++i)
        {
            for (auto j = 0u; j < bytes; ++j)
            {
                if (!compiler.available_heap[i + j])
                {
                    available = false;
                }
            }
            
            if (available)
            {
                for (auto k = 0u; k < bytes; ++k)
                {
                    compiler.available_heap[i + k] = false;
                }
                
                address = i;
            }
            
            else available = true;
        }
        
        ti::throw_error(fmt::format("No contiguous heap region of {} bytes available", bytes));
    }
    
    HeapAllocation::~HeapAllocation(void) noexcept
    {
        /*
        may want a flag in the struct to determine of the memory should be freed after destruction
         ->strings should not be deallocated ever (they are in constant memory, which is faked on the heap directly)m
         
        for (auto i = address; i < address + bytes; ++i)
        {
            context.available_heap[i] = true;
        }
         */
    }
}
