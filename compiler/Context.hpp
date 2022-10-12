#ifndef Context_hpp
#define Context_hpp

#include <vector>
#include <forward_list>
#include <array>
#include <string>
#include <optional>
#include <ciso646>


#include "Types.hpp"
#include "IR.hpp"
#include "Expression.hpp"

#define common2 { common.context, common.parent_function, new_allocation.allocation }

namespace ti
{
    struct Argument;
    
    enum class SymbolType
    {
        VARIABLE,
        FUNCTION,
    };
    
    namespace sym
    {
        struct Variable
        {
            std::uint16_t address;
            TypeVisibility visibility;
            Function& parent_function;
        };
        
        struct Function
        {
            std::vector<Argument> arguments;
        };
    }
    
    struct Symbol
    {
        SymbolType type;
        std::string name;
        bool defined;
        
        const union
        {
            sym::Variable variable;
            sym::Function function;
        } as;
    };
    

    
    
    
    
    struct Context
    {
        std::vector<Symbol*> symbol_table;
        std::forward_list<Command> ir_code;
        
        std::array<bool, 4> available_registers;
        std::array<bool, 0x4000> available_heap;
        
        Context(void) noexcept
        {
            symbol_table = {};
            ir_code = {};
            
            for (auto&& reg : available_registers)
                reg = true;
            
            for (auto&& byte : available_heap)
                byte = true;
        }
        
        const Allocation allocate_register(void) noexcept;
        const Allocation allocate_register_forced(void) noexcept;
        const Allocation allocate_register_forced_exclude(const Allocation&) noexcept;
        
        void deallocate_register(const Location&) noexcept;
        void deallocate_register(const Allocation&) noexcept;
        
        std::uint16_t allocate_heap(std::uint16_t) noexcept;
        void deallocate_heap(std::uint16_t, std::uint16_t) noexcept;
    };
    
    

    
    enum RegisterType
    {
        R0,
        R1,
        R2,
        R3,
    };
    
    struct RegisterAllocation
    {
        RegisterType location;
        Context& context;
        
        RegisterAllocation(Context& context) noexcept
            : context(context)
        {
            location = context.allocate_register();
        }

        ~RegisterAllocation(void) noexcept
        {
            context.deallocate_register(location);
        }
    };
    
    struct ForcedRegisterAllocation
    {
        RegisterType location;
        Context& context;
        
        ForcedRegisterAllocation(Context& context) noexcept
            : context(context)
        {
            location = context.allocate_register_forced();
        }
        
        ForcedRegisterAllocation(Context& context, RegisterType exclude) noexcept
            : context(context)
        {
            location = context.allocate_register_forced_exclude(exclude);
        }
        
        
        ~ForcedRegisterAllocation(void) noexcept
        {
            context.deallocate_register_forced(location);
        }
    };
    
    struct HeapAllocation
    {
        std::uint16_t address, size;
        
        HeapAllocation(Context& context, std::uint16_t size) noexcept
            : context(context), size(size)
        {
            address = context.allocate_heap(size);
        }
        
        ~HeapAllocation(void) noexcept
        {
            context.deallocate_heap(address, size);
        }
    };
    
    /*
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
            
            for (auto&& r : available_registers)
                r = true;
            
            for (auto&& h : available_heap)
                h = true;
            
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
        
    };*/
    
    std::uint8_t get_size_by_type(CompleteType) noexcept;
    std::string&& location_to_string(Location) noexcept;
    std::string&& expression_type_to_string(ExpressionType) noexcept;
}



#endif /* Context_hpp */
