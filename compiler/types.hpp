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
        WORD,
        VOID,
    };
    
    enum class TypeQualifier
    {
        PTR,
        VAL,
    };
    
    struct CompleteType
    {
        TypeSpecifier specifier;
        TypeQualifier qualifier;
    }
}


enum class TypeVisibility
{
    
}

#endif /* types_h */
