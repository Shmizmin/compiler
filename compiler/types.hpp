#ifndef types_h
#define types_h

namespace ti
{
    struct Context;
    struct Function;
    struct Allocation;
    
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
        Allocation& allocation;
    };
}

#endif /* types_h */
