#ifndef Context_hpp
#define Context_hpp

#include <vector>
#include <array>
#include <string>
#include <optional>

#include "Types.hpp"
#include "Expression.hpp"

namespace ti
{
    enum class SymbolType
    {
        VARIABLE,
        FUNCTION,
    };
    
    struct Symbol
    {
        SymbolType type;
        std::string name;
        bool defined;
    };
    
    struct VariableSymbol : public Symbol
    {
        std::uint16_t address;
        TypeVisibility visibility;
        Function& function; //parent function
    };
    
    struct FunctionSymbol : public Symbol
    {
        //null
        std::string label;
    };
    
    
    
    enum Location
    {
        R0,
        R1,
        R2,
        R3,
        STACK,
        HEAP,
    };
    
    struct ForcedAllocation
    {
        Location location;
        bool was_forced; //requires 'pop'ing old value back into reg
    };
    
    struct Context
    {
        std::vector<Symbol*> symbol_table;
        std::string code_segment;
        std::string end_segment;
        std::array<bool, 4> available_registers;
        std::array<bool, 0x4000> available_heap;
        std::uint32_t counter;
        std::uint16_t stack_pointer;
        
        Context(void)
        {
            symbol_table = {};
            code_segment = "";
            available_registers = { true };
            available_heap = { true };
            counter = 0u;
            stack_pointer = 0x7FFF;
        }

        void add_to_symbol_table(Symbol*) noexcept;
        
        void add_to_code(const std::string&) noexcept;
        void add_to_end(const std::string&) noexcept;
        
        const ForcedAllocation allocate_forced(void) noexcept;
        const ForcedAllocation allocate_forced(const ForcedAllocation&) noexcept;
        void deallocate_forced(const ForcedAllocation&) noexcept;
        
        const Location allocate(void) noexcept;
        void deallocate(const Location&) noexcept;
        
        std::uint16_t allocate_heap(std::uint16_t) noexcept;
        void deallocate_heap(std::uint16_t, std::uint16_t) noexcept;
        
    };
    
    std::uint8_t get_type_size(CompleteType&) noexcept;
    std::string location_to_string(Location) noexcept;
    std::string expression_type_to_string(ExpressionType) noexcept;
}



#endif /* Context_hpp */
