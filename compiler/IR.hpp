#ifndef IR_hpp
#define IR_hpp

#include <cstdint>
#include <limits>
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
            std::string text{};
        };
    }
    
    struct Directive
    {
        DirectiveType type;
        
        union
        {
            dir::Origin origin;
            dir::Byte byte;
            dir::Word word;
            dir::Ascii ascii;
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
        struct IP {};
        
        struct REG
        {
            RegisterType location;
        };
        
        struct IMM
        {
            std::uint8_t value;
        };
        
        struct MEM
        {
            std::uint16_t address;
        };
    }
    
    struct Operand
    {
        OperandType type;
        
        union
        {
            op::IP ip;
            op::REG reg;
            op::IMM imm;
            op::MEM mem;
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
        struct Nop {};
        struct Brk {};
        
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
                JCC,
            } cond;
            
            std::string label{};
        };
        
        struct Stack
        {
            enum class Data
            {
                A, B, C, D, F,
                IP,
            } data;
            
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
        InstructionType type;
        
        union
        {
            insn::Nop nop;
            insn::Brk brk;
            insn::Math math;
            insn::Move move;
            insn::Jmp jmp;
            insn::Stack stack;
            insn::Deref deref;
        } as;
    };
    
    
    
    struct Label
    {
        std::string name{};
    };
    
    
    
    
    enum CommandType
    {
        DIRECTIVE,
        INSTRUCTION,
        LABEL,
    };
    
    struct Command
    {
        CommandType type;
        
        union
        {
            Directive directive;
            Instruction instruction;
            Label label;
        } as;
    };
    
    std::string generate_commands(const std::list<Command*>&) noexcept;
}

#endif
