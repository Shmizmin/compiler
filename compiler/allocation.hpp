#ifndef allocation_hpp
#define allocation_hpp

#include <cstdint>

namespace ti
{
    struct Compiler;
    enum class RegisterType;

    struct RegisterAllocation
    {
        bool was_forced;
        RegisterType location;
        Compiler& context;
        
        RegisterAllocation(Compiler&) noexcept;
        RegisterAllocation(Compiler&, RegisterType) noexcept;
        ~RegisterAllocation(void) noexcept;
    };

    struct HeapAllocation
    {
        std::uint16_t address, bytes;
        Compiler& context;
        
        HeapAllocation(Compiler&, std::uint16_t) noexcept;
        ~HeapAllocation(void) noexcept;
    };
}

#endif
