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
        FCALL,
        
        EQUALS,

        PLUS,
        MINUS,

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
            std::uint8_t value;
            
            void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
        };
        
        struct Stringconst : public Expression
        {
            std::string value;
            
            void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
        };
        
        struct Identifier : public Expression
        {
            std::string name;
            
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
        };
        
        namespace binary
        {
            struct FCall : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            
            struct Equals : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            
            struct Plus : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            struct Minus : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            
            struct LeftShift : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            struct RightShift : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            
            struct BitXor : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            struct BitAnd : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            struct BitOr : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            
            struct LogAnd : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            struct LogOr : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            
            struct EqualsEquals : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            struct NotEquals : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            
            struct Less : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
            struct Greater : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            };
        }
        

        struct Unary : public Expression
        {
            Expression* center;
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
