#ifndef types_h
#define types_h

namespace ti
{
    struct Context;
    struct Function;
    enum class RegisterType;
    
    enum class TypeVisibility
    {
        LOCAL,
        GLOBAL,
    };
        
    enum class TypeSpecifier
    {
        BYTE,
        VOID,
    };
    
    enum class TypeQualifier
    {
        VAL,
    };
    
    struct CompleteType
    {
        TypeSpecifier specifier;
        TypeQualifier qualifier;
    };
    
    struct CommonArgs
    {
        Context& context;
        Function& parent_function;
        RegisterType& allocation;
    };
    
    enum class RegisterType
    {
        R0,
        R1,
        R2,
        R3,
        RF,
        IP,
    };
}

#endif
