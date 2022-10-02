#ifndef Expression_hpp
#define Expression_hpp

#include <string>
#include <string_view>
#include <vector>

#include "Types.hpp"

namespace ti
{
    struct Context;
    struct Function;
    struct ForcedAllocation;
    
    enum class ExpressionType
    {
        NUMCONST,
        STRINGCONST,
        IDENTIFIER,
        FUNCTION_CALL,
        TERNARYOP,
        BINARYOP,
        UNARYOP,
    };
    
    enum class BinaryOperator
    {
        FCALL,
        
        EQUALS,

        PLUS,
        MINUS,

        LEFT_SHIFT,
        RIGHT_SHIFT,

        BIT_XOR,
        BIT_AND,
        BIT_OR,
        
        EQUALS_EQUALS,
        NOT_EQUALS,

        LESS,
        GREATER,
    };
    
    enum class UnaryOperator
    {
        PLUS_PLUS,
        MINUS_MINUS,

        POSITIVE,
        NEGATIVE,
    };
    
    struct Expression;
    
    namespace expr
    {
        struct Numconst
        {
            std::uint8_t value;
        };
        
        struct Stringconst
        {
            std::string text;
        };
        
        struct Identifier
        {
            std::string name;
        };
        
        struct Function_Call
        {
            Identifier left;
            std::vector<Expression*> args;
        };
        
        struct Ternaryop
        {
            Expression *left, *center, *right;
        };
        
        struct Binaryop
        {
            const BinaryOperator type;
            
            Expression *left, *right;
        };
        
        struct Unaryop
        {
            const UnaryOperator type;
            
            Expression *center;
        };
    }
        
    struct Expression
    {
        const ExpressionType type;
        
        const union
        {
            expr::Numconst numconst;
            expr::Stringconst stringconst;
            expr::Identifier identifier;
            expr::Function_Call functioncall;
            expr::Ternaryop ternaryop;
            expr::Binaryop binaryop;
            expr::Unaryop unaryop;
        } as;
    };
    
    struct CommonArgs
    {
        Context& context;
        Function& function;
        ForcedAllocation& allocation;
    };
    
    
    Expression make_numconst(std::uint8_t) noexcept;
    Expression make_stringconst(std::string&&) noexcept;
    Expression make_identifier(std::string&&) noexcept;
    Expression make_function_call(std::string&&, std::vector<Expression>&&) noexcept;
    Expression make_ternaryop(Expression*, Expression*, Expression*) noexcept;
    Expression make_binaryop(Expression*, Expression*, BinaryOperator) noexcept;
    Expression make_unaryop(Expression*, UnaryOperator) noexcept;
    
    void compile_expression(Expression&, CommonArgs&) noexcept;
}

#endif /* Expression_hpp */
