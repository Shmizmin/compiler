#ifndef Expression_hpp
#define Expression_hpp

#include <string>
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
        FCALL,
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
        
        EQUALS_EQUALS,
        NOT_EQUALS,

        LESS,
        GREATER,
    };
    
    enum class UnaryOp
    {
        PLUS_PLUS,
        MINUS_MINUS,

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
            //CompleteType complete_type;
            std::uint8_t value;
            
            void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            
            Numconst(std::uint8_t value)
                : value(value)
            {
                type = ti::ExpressionType::NUMCONST;
            }
        };
        
        struct Stringconst : public Expression
        {
            std::string value;
            
            void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            
            Stringconst(std::string value)
                : value(value)
            {
                type = ti::ExpressionType::STRINGCONST;
            }
        };
        
        struct Identifier : public Expression
        {
            std::string name;
            
            void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            
            Identifier(std::string name)
                : name(name)
            {
                type = ti::ExpressionType::IDENTIFIER;
            }
        };
        
        struct FCall : public Expression
        {
            Identifier* left;
            std::vector<Expression*> args;
            
            void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            
            FCall(Identifier* left, std::vector<Expression*> args)
                : left(left), args(args)
            {
                type = ti::ExpressionType::FCALL;
            }
        };
        
        struct Ternary : public Expression
        {
            Expression* left;
            Expression* center;
            Expression* right;
            
            void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
            
            Ternary(Expression* left, Expression* center, Expression* right)
                : left(left), center(center), right(right)
            {
                type = ti::ExpressionType::TERNARYOP;
            }
        };
        
        struct Binary : public Expression
        {
            Expression* left;
            Expression* right;
            
            Binary(Expression* left, Expression* right)
                : left(left), right(right)
            {
                type = ti::ExpressionType::BINARYOP;
            }
        };
        
        namespace binary
        {
            struct Equals final : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                Equals(Expression* left, Expression* right)
                    : Binary(left, right)
                {
                }
            };
            
            
            struct Plus final : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                Plus(Expression* left, Expression* right)
                    : Binary(left, right)
                {
                }
            };
            struct Minus final : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                Minus(Expression* left, Expression* right)
                    : Binary(left, right)
                {
                }
            };
            
            struct LeftShift final : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                LeftShift(Expression* left, Expression* right)
                    : Binary(left, right)
                {
                }
            };
            struct RightShift final : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                RightShift(Expression* left, Expression* right)
                    : Binary(left, right)
                {
                }
            };
            
            struct BitXor final : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                BitXor(Expression* left, Expression* right)
                    : Binary(left, right)
                {
                }
            };
            struct BitAnd final : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                BitAnd(Expression* left, Expression* right)
                    : Binary(left, right)
                {
                }
            };
            struct BitOr final : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                BitOr(Expression* left, Expression* right)
                    : Binary(left, right)
                {
                }
            };
            
            struct EqualsEquals final : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                EqualsEquals(Expression* left, Expression* right)
                    : Binary(left, right)
                {
                }
            };
            struct NotEquals final : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                NotEquals(Expression* left, Expression* right)
                    : Binary(left, right)
                {
                }
            };
            
            struct Less final : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                Less(Expression* left, Expression* right)
                    : Binary(left, right)
                {
                }
            };
            struct Greater final : public Binary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                Greater(Expression* left, Expression* right)
                    : Binary(left, right)
                {
                }
            };
        }
        

        struct Unary : public Expression
        {
            Expression* center;
            
            Unary(Expression* center)
                : center(center)
            {
                type = ti::ExpressionType::UNARYOP;
            }
        };
       
        namespace unary
        {
            struct PlusPlus final : public Unary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                PlusPlus(Expression* center)
                    : Unary(center)
                {
                }
            };
            struct MinusMinus final : public Unary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                MinusMinus(Expression* center)
                    : Unary(center)
                {
                }
            };
            struct Positive final : public Unary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                Positive(Expression* center)
                    : Unary(center)
                {
                }
            };
            struct Negative final : public Unary
            {
                void generate(Context&, Function&, const ForcedAllocation&) noexcept override;
                
                Negative(Expression* center)
                    : Unary(center)
                {
                }
            };
        }
    }
}

#endif /* Expression_hpp */
