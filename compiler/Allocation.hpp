#ifndef Allocation_hpp
#define Allocation_hpp

#include <cstdint>

namespace ti
{
    struct Context;
    enum class RegisterType;

    struct RegisterAllocation
    {
        RegisterType location;
        Context& context;
        
        RegisterAllocation(Context&) noexcept;
        RegisterAllocation(Context&, RegisterType) noexcept;
        ~RegisterAllocation(void) noexcept;
    };

    struct HeapAllocation
    {
        std::uint16_t address, bytes;
        Context& context;
        
        HeapAllocation(Context&, std::uint16_t) noexcept;
        ~HeapAllocation(void) noexcept;
    };
}

#endif
