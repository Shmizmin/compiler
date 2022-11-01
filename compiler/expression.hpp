#ifndef expression_hpp
#define expression_hpp

#include <string>
#include <string_view>
#include <vector>

#include "types.hpp"

namespace ti
{
    struct Compiler;
    struct Function;
    struct RegisterAllocation;
    
    enum class ExpressionType
    {
        NUMCONST,
        STRINGCONST,
        IDENTIFIER,
        FUNCTIONCALL,
        TERNARYOP,
        BINARYOP,
        UNARYOP,
    };
    
    enum class BinaryOperator
    {
        EQUALS,

        PLUS,
        MINUS,

        LEFT_SHIFT,
        RIGHT_SHIFT,

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
        
        struct FunctionCall
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
        const Function* parent_function;
        
        const union As
        {
            expr::Numconst numconst;
            expr::Stringconst stringconst;
            expr::Identifier identifier;
            expr::FunctionCall functioncall;
            expr::Ternaryop ternaryop;
            expr::Binaryop binaryop;
            expr::Unaryop unaryop;
            
            As(void) noexcept {}
            ~As(void) noexcept {}
        } as;
    };
    
    Expression* make_numconst(std::uint8_t) noexcept;
    Expression* make_stringconst(std::string&&) noexcept;
    Expression* make_identifier(std::string&&) noexcept;
    Expression* make_functioncall(std::string&&, std::vector<Expression*>&&) noexcept;
    Expression* make_ternaryop(Expression*, Expression*, Expression*) noexcept;
    Expression* make_binaryop(Expression*, Expression*, BinaryOperator) noexcept;
    Expression* make_unaryop(Expression*, UnaryOperator) noexcept;
    
    void compile_expression(Expression*, Compiler&, const RegisterAllocation&) noexcept;
}

#endif
