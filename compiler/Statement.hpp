#ifndef Statement_hpp
#define Statement_hpp

#include <vector>

#include "Expression.hpp"
#include "Types.hpp"
#include "Context.hpp"

namespace ti
{
    //avoid circular dependency
    struct Context;
    struct Function;
    
    
    struct Variable
    {
        TypeVisibility visibility;
        CompleteType type;
        std::string name;
        Expression* value;
    };
    
    enum class StatementType
    {
        BLOCK,
        IF,
        WHILE,
        RETURN,
        NIL,
        VARIABLE,
    };
    
    
    struct Statement
    {
        StatementType type;
        
        virtual void generate(Context&, Function&) noexcept = 0;
    };
    
    namespace stmt
    {
        struct Block final : public Statement
        {
            std::vector<Statement*> statements;
            
            void generate(Context&, Function&) noexcept override;
        };
        
        struct If final : public Statement
        {
            Expression* condition;
            Statement* statement;
            
            void generate(Context&, Function&) noexcept override;
        };
        
        struct While final : public Statement
        {
            Expression* condition;
            Statement* statement;
            
            void generate(Context&, Function&) noexcept override;
        };
        
        struct Return final : public Statement
        {
            Expression* value;
            
            void generate(Context&, Function&) noexcept override;
        };
        
        struct Null final : public Statement
        {
            //null
            
            void generate(Context&, Function&) noexcept override;
        };
        
        struct Variable final : public Statement
        {
            std::vector<ti::Variable> variables;
            
            void generate(Context&, Function&) noexcept override;
        };
    }
}

#endif /* Statement_hpp */
