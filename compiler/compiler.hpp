#ifndef CompilerContext_hpp
#define CompilerContext_hpp

#include <vector>
#include <list>
#include <array>
#include <string>

#include "types.hpp"
#include "bytecode.hpp"
#include "expression.hpp"
#include "function.hpp"

namespace ti
{
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
            std::vector<Argument> arguments;
        };
    }
    
    struct Symbol
    {
        const SymbolType type;
        std::string name;
        bool defined;
        
        const union As
        {
            sym::Variable variable;
            sym::Function function;
            
            As(void) noexcept : function({}) {}
            ~As(void) noexcept {}
        } as;
    };
    

    int generate_uuid(void) noexcept;
    
    enum class RegisterType;
    class HeapAllocation;
    
    class Compiler
    {
    public:
        std::vector<Symbol*> symbol_table;
        std::list<Command*> ir_code;
        
        std::array<bool, 4> available_registers;
        std::array<bool, 0x4000> available_heap;
        
        Compiler(void) noexcept
        {
            symbol_table = {};
            ir_code = {};
            
            for (auto& reg : available_registers)
            {
                reg = true;
            }
                
            for (auto& byte : available_heap)
            {
                byte = true;
            }
        }
        
        void add_to_symbol_table(Symbol*) noexcept;
        
        
        void emit_label(const std::string&) noexcept;
        void emit_ascii(const std::string&) noexcept;
        
        void emit_org(std::uint16_t) noexcept;
        void emit_org(const HeapAllocation&) noexcept;
        
        void emit_jmp(insn::Jmp::Condition, const std::string&) noexcept;
        
        void emit_adc(const RegisterAllocation&, std::uint8_t) noexcept;
        void emit_adc(const RegisterAllocation&, const RegisterAllocation&) noexcept;
        
        void emit_sbb(const RegisterAllocation&, std::uint8_t) noexcept;
        void emit_sbb(const RegisterAllocation&, const RegisterAllocation&) noexcept;
        
        void emit_and(const RegisterAllocation&, std::uint8_t) noexcept;
        void emit_and(const RegisterAllocation&, const RegisterAllocation&) noexcept;
        
        void emit_lor(const RegisterAllocation&, std::uint8_t) noexcept;
        void emit_lor(const RegisterAllocation&, const RegisterAllocation&) noexcept;
        
        void emit_rol(const RegisterAllocation&, std::uint8_t) noexcept;
        void emit_ror(const RegisterAllocation&, std::uint8_t) noexcept;
        
        void emit_not(const RegisterAllocation&) noexcept;
        
        void emit_stb(std::uint16_t, const RegisterAllocation&) noexcept;
        void emit_stb(const HeapAllocation&, const RegisterAllocation&) noexcept;
        void emit_mvb(const RegisterAllocation&, const RegisterAllocation&) noexcept;
        void emit_ldb(const RegisterAllocation&, std::uint8_t) noexcept;
        
        void emit_call(const RegisterAllocation&, const std::string&) noexcept;
        
        void emit_push(const RegisterAllocation&) noexcept;
        void emit_pop(const RegisterAllocation&) noexcept;
    };
    
    std::uint8_t get_size_by_type(CompleteType) noexcept;
}

#endif
