#ifndef allocation_hpp
#define allocation_hpp

#include <cstdint>

namespace ti
{
    struct Compiler;
    enum class RegisterType;

    class RegisterAllocation
    {
    private:
        friend class Compiler;
        bool was_forced;
        RegisterType location;
        Compiler& compiler;
        
    public:
        //meant for temporaries only
        // TODO: refactor to allow to be private
        RegisterAllocation(bool, RegisterType, Compiler&) noexcept;
        
        RegisterAllocation(Compiler&) noexcept;
        RegisterAllocation(Compiler&, const RegisterAllocation&) noexcept;
        ~RegisterAllocation(void) noexcept;
    };

    class HeapAllocation
    {
    private:
        friend class Compiler;
        std::uint16_t address, bytes;
        Compiler& compiler;
        
    public:
        HeapAllocation(Compiler&, std::uint16_t) noexcept;
        ~HeapAllocation(void) noexcept;
        
    public:
        friend std::uint16_t operator&(const HeapAllocation&, std::uint16_t) noexcept;
    };
    
    std::uint16_t operator&(const HeapAllocation& allocation, std::uint16_t bitmask) noexcept
    {
        return (allocation.address & bitmask);
    }
}

#endif
