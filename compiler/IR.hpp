#ifndef IR_hpp
#define IR_hpp

#include <cstdint>

namespace ti
{
#pragma mark Init
    enum class DirectiveType
    {
        ORIGIN,
        BEGIN,
        END,
        BYTE,
        WORD,
        ASCII,
    };
    
    struct Directive;
    
    namespace dir
    {
        struct Origin
        {
            
        };
        
        struct Begin
        {
            
        };
        
        struct End
        {
            
        };
        
        struct Byte
        {
            
        };
        
        struct Word
        {
            
        };
        
        struct Ascii
        {
            
        };
    }
    
    struct Directive
    {
        const DirectiveType type;
        
        const union
        {
            dir::Origin origin;
            dir::Begin begin;
            dir::End end;
            dir::Byte byte;
            dir::Word word;
            dir::Ascii ascii;
        } as;
    };
    
    
    
    enum class InstructionType
    {
        NOP,
        BRK,
        MVB,
        ADC,
        SBB,
        AND,
        LOR,
        NOT,
        ROL,
        ROR,
        LDB,
        STB,
        JEZ,
        JCS,
        PUSH,
        POP,
        DEREF,
    };
    
    struct Instruction;
    
    namespace insn
    {
        struct Nop
        {
            
        };
        
        struct Brk
        {
            
        };
        
        struct Mvb
        {
            
        };
        
        struct Adc
        {
            
        };
        
        struct Sbb
        {
            
        };
        
        struct Land
        {
            
        };
        
        struct Lor
        {
            
        };
        
        struct Lnot
        {
            
        };
        
        struct Rol
        {
            
        };
        
        struct Ror
        {
            
        };
        
        struct Ldb
        {
            
        };
        
        struct Stb
        {
            
        };
        
        
        struct Jmp
        {
            enum
            {
                JEZ,
                JCS,
            } type;
            
            std::uint16_t addr;
        };
        
        struct Push
        {
            enum
            {
                A, B, C, D, F,
                IP,
            } op;
        };
        
        struct Pop
        {
            enum
            {
                A, B, C, D, F,
                IP,
                DISCARD,
            } op;
        };
        
        struct Deref
        {
            enum
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
            insn::Mvb mvb;
            insn::Adc adc;
            insn::Sbb sbb;
            insn::Land land;
            insn::Lor lor;
            insn::Lnot lnot;

        } as;
    };
    
    
    
    
    
    struct Command
    {
        const CommandType type;
        
        const union
        {
            
        } as;
    };
}


#endif /* IR_hpp */
