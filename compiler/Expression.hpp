#ifndef Expression_hpp
#define Expression_hpp

#include <string>
#include <variant>

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
        
        virtual void generate(Context&, Function&, const ForcedAllocation&) noexcept = 0;
    };
    
    namespace expr
    {
        struct Numconst : public Expression
        {
            CompleteType complete_type;
            std::variant<std::uint8_t, std::uint16_t> value;
            
            void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
        };
        
        struct Stringconst : public Expression
        {
            std::string value;
            
            void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
        };
        
        struct Identifier : public Expression
        {
            std::string identifier;
            
            void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
        };
        
        struct Ternary : public Expression
        {
            Expression* left;
            Expression* center;
            Expression* right;
            
            void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
        };
        
        struct Binary : public Expression
        {
            Expression* left;
            Expression* right;
            BinaryOp op;
            
            void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
        };
        
        namespace binary
        {
            
        }
        
        
        
        
        
        
        struct Unary : public Expression
        {
            Expression* center;
            
            //void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
        };
        
        namespace unary
        {
            struct PlusPlus : public Unary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            struct MinusMinus : public Unary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            struct Addrof : public Unary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            struct Deref : public Unary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            struct Positive : public Unary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            struct Negative : public Unary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
        }
    }
}

#endif /* Expression_hpp */
