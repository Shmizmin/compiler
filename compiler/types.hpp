#ifndef types_h
#define types_h

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
}

#endif /* types_h */
