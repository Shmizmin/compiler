#ifndef types_hpp
#define types_hpp

namespace ti
{
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
    
    
    struct Compiler;
    struct Function;
    enum class RegisterType;
    
    struct CommonArgs
    {
        Compiler& context;
        Function* parent_function;
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
