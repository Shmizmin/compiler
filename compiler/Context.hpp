#ifndef Context_hpp
#define Context_hpp

#include <vector>
#include <list>
#include <optional>
#include <array>
#include <string>
#include <optional>
#include <ciso646>


#include "Types.hpp"
#include "IR.hpp"
#include "Expression.hpp"

#define common2 { common.context, common.parent_function, new_allocation.location }

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
            Function* parent_function;
        };
        
        struct Function
        {
            std::vector<Argument> arguments{};
        };
    }
    
    struct Symbol
    {
        SymbolType type;
        std::string name;
        bool defined;
        
        union
        {
            sym::Variable variable;
            sym::Function function;
        } as;
    };
    

    
    int generate_uuid(void) noexcept;
    
    
    enum class RegisterType;
    
    struct Context
    {
        std::vector<Symbol*> symbol_table;
        std::list<Command*> ir_code;
        
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
        
        void add_to_symbol_table(Symbol*) noexcept;
        
        std::optional<RegisterType> allocate_register(void) noexcept;
        RegisterType allocate_register_forced(void) noexcept;
        RegisterType allocate_register_forced_exclude(RegisterType) noexcept;
        
        void deallocate_register(RegisterType) noexcept;
        void deallocate_register_forced(RegisterType) noexcept;
        
        std::uint16_t allocate_heap(std::uint16_t) noexcept;
        void deallocate_heap(std::uint16_t, std::uint16_t) noexcept;
        
        
        void emit_label(const std::string&) noexcept;
        void emit_ascii(const std::string&) noexcept;
        void emit_org(std::uint16_t) noexcept;
        
        void emit_jmp(insn::Jmp::Condition, const std::string&) noexcept;
        
        void emit_adc(RegisterType, std::uint8_t) noexcept;
        void emit_adc(RegisterType, RegisterType) noexcept;
        
        void emit_sbb(RegisterType, std::uint8_t) noexcept;
        void emit_sbb(RegisterType, RegisterType) noexcept;
        
        void emit_and(RegisterType, std::uint8_t) noexcept;
        void emit_and(RegisterType, RegisterType) noexcept;
        
        void emit_lor(RegisterType, std::uint8_t) noexcept;
        void emit_lor(RegisterType, RegisterType) noexcept;
        
        void emit_rol(RegisterType, std::uint8_t) noexcept;
        void emit_ror(RegisterType, std::uint8_t) noexcept;
        
        void emit_not(RegisterType) noexcept;
        
        void emit_xor(RegisterType, RegisterType) noexcept;
        
        void emit_stb(std::uint16_t, RegisterType) noexcept;
        void emit_mvb(RegisterType, RegisterType) noexcept;
        void emit_ldb(RegisterType, std::uint8_t) noexcept;
        
        void emit_call(RegisterType, std::string&&) noexcept;
        
        void emit_push(RegisterType) noexcept;
        void emit_pop(RegisterType) noexcept;
    };
    
    
    struct RegisterAllocation
    {
        RegisterType location;
        Context& context;
        
        RegisterAllocation(Context& context) noexcept
            : context(context)
        {
            location = context.allocate_register().value();
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
        Context& context;
        
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
    
    std::uint8_t get_size_by_type(CompleteType) noexcept;
    std::string regtype_to_string(RegisterType) noexcept;
}

#endif
