#ifndef Bytecode_hpp
#define Bytecode_hpp

#include <cstdint>
#include <string>
#include <list>

namespace ti
{
    enum class DirectiveType
    {
        ORIGIN,
        BYTE,
        WORD,
        ASCII,
    };
    
    struct Directive;
    enum class RegisterType;
    
    namespace dir
    {
        struct Origin
        {
            std::uint16_t address;
        };
        
        struct Byte
        {
            std::uint8_t value;
        };
        
        struct Word
        {
            std::uint16_t value;
        };
        
        struct Ascii
        {
            std::string text;
        };
    }
    
    struct Directive
    {
        const DirectiveType type;
        
        const union As
        {
            dir::Origin origin;
            dir::Byte byte;
            dir::Word word;
            dir::Ascii ascii;
            
            As(void) noexcept {}
            ~As(void) noexcept {}
        } as;
    };
    
    
    
    enum class OperandType
    {
        REG,
        IP,
        IMM, MEM,
    };
    
    struct Operand;
    
    namespace op
    {
        struct Reg
        {
            RegisterType location;
        };
        
        struct Imm
        {
            std::uint8_t value;
        };
        
        struct Mem
        {
            std::uint16_t address;
        };
    }
    
    struct Operand
    {
        const OperandType type;
        
        const union As
        {
            op::Reg reg;
            op::Imm imm;
            op::Mem mem;
            
            As(void) noexcept {}
            ~As(void) noexcept {}
        } as;
    };

    
    
    enum class InstructionType
    {
        NOP,
        BRK,
        MATH,
        MOVE,
        JMP,
        STACK,
        DEREF,
    };
    
    struct Instruction;
    
    namespace insn
    {
        struct Math
        {
            enum class Operation
            {
                ADC,
                SBB,
                LAND,
                LOR,
                LNOT,
                ROL,
                ROR,
            } op;
            
            Operand op1, op2;
        };
        
        struct Move
        {
            Operand op1, op2;
        };
        
        
        struct Jmp
        {
            enum class Condition
            {
                JEZ,
                JCS,
            } cond;
            
            std::string label;
        };
        
        struct Stack
        {
            Operand op;
            
            enum class Direction
            {
                PUSH,
                POP,
            } dir;
        };
        
        struct Deref
        {
            enum class Operation
            {
                AB_TO_A,
                CD_TO_C,
            } op;
        };
    }
    
    struct Instruction
    {
        const InstructionType type;
        
        const union As
        {
            insn::Math math;
            insn::Move move;
            insn::Jmp jmp;
            insn::Stack stack;
            insn::Deref deref;
            
            As(void) noexcept {}
            ~As(void) noexcept {}
        } as;
    };
    
    
    
    struct Label
    {
        std::string name;
    };
    
    
    
    
    enum CommandType
    {
        DIRECTIVE,
        INSTRUCTION,
        LABEL,
    };
    
    struct Command
    {
        const CommandType type;
        
        const union As
        {
            Directive directive;
            Instruction instruction;
            Label label;
            
            As(void) noexcept {}
            ~As(void) noexcept {}
        } as;
    };
    
    std::string generate_commands(const std::list<Command*>&) noexcept;
}

#endif
