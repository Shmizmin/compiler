#ifndef IR_hpp
#define IR_hpp

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

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
        
        const union
        {
            dir::Origin origin;
            dir::Byte byte;
            dir::Word word;
            dir::Ascii ascii;
        } as;
    };
    
    
    
    enum class OperandType
    {
        A, B, C, D, F,
        IP,
        IMM, MEM,
    };
    
    struct Operand;
    
    namespace op
    {
        struct A  {};
        struct B  {};
        struct C  {};
        struct D  {};
        struct F  {};
        struct IP {};
        
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
        const OperandType type;
        
        const union
        {
            op::A a;
            op::B b;
            op::C c;
            op::D d;
            op::F f;
            op::IP ip;
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
            
            std::string label;
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
        const InstructionType type;
        
        const union
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
        
        const union
        {
            Directive directive;
            Instruction instruction;
            Label label;
        } as;
    };
    
    std::string generate_commands(const std::vector<Command>&) noexcept;
}


#endif /* IR_hpp */
