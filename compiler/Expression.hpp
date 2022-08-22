#ifndef Expression_hpp
#define Expression_hpp

#include <string>
#include <variant>

#include "Types.hpp"

namespace ti
{
    enum class ExpressionType
    {
        NUMCONST,
        STRINGCONST,
        IDENTIFIER,
        TERNARYOP,
        BINARYOP,
        UNARYOP,
    };
    
    enum class BinaryOp
    {
        INDEX,
        FCALL,
        
        EQUALS,

        PLUS,
        MINUS,

        PLUS_EQUALS,
        MINUS_EQUALS,

        LEFT_SHIFT,
        RIGHT_SHIFT,

        BIT_XOR,
        BIT_AND,
        BIT_OR,

        LOG_AND,
        LOG_OR,

        EQUALS_EQUALS,
        NOT_EQUALS,

        LESS,
        GREATER,
        LESS_EQUALS,
        GREATER_EQUALS,
    };
    
    enum class UnaryOp
    {
        PLUS_PLUS,
        MINUS_MINUS,
        
        ADDROF,
        DEREF,

        POSITIVE,
        NEGATIVE,
    };
    
    struct Expression
    {
        ExpressionType type;
    };
    
    struct NumconstExpression : public Expression
    {
        CompleteType complete_type;
        std::variant<std::uint8_t, std::uint16_t> value;
    };
    
    struct StringconstExpression : public Expression
    {
        std::string value;
    };
    
    struct IdentifierExpression : public Expression
    {
        std::string identifier;
    };
    
    struct TernaryExpression : public Expression
    {
        Expression* left, center, right;
    };
    
    struct BinaryExpression : public Expression
    {
        Expression* left, right;
        BinaryOp op;
    };
    
    struct UnaryExpression : public Expression
    {
        Expression* center;
        UnaryOp op;
    };
}

#endif /* Expression_hpp */
