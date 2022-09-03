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
            
            Block(std::vector<Statement*> statements) noexcept
                : statements(statements)
            {
                type = ti::StatementType::BLOCK;
            }
        };
        
        struct If final : public Statement
        {
            Expression* condition;
            Statement* statement;
            
            void generate(Context&, Function&) noexcept override;
            
            If(Expression* condition, Statement* statement) noexcept
                : condition(condition), statement(statement)
            {
                type = ti::StatementType::IF;
            }
        };
        
        struct While final : public Statement
        {
            Expression* condition;
            Statement* statement;
            
            void generate(Context&, Function&) noexcept override;
            
            While(Expression* condition, Statement* statement) noexcept
                : condition(condition), statement(statement)
            {
                type = ti::StatementType::WHILE;
            }
        };
        
        struct Return final : public Statement
        {
            Expression* value;
            
            void generate(Context&, Function&) noexcept override;
            
            Return(Expression* value) noexcept
                : value(value)
            {
                type = ti::StatementType::RETURN;
            }
        };
        
        struct Null final : public Statement
        {
            //null
            
            void generate(Context&, Function&) noexcept override;
            
            Null(void) noexcept
            {
                type = ti::StatementType::NIL;
            }
        };
        
        struct Variable final : public Statement
        {
            std::vector<ti::Variable*> variables;
            
            void generate(Context&, Function&) noexcept override;
            
            Variable(std::vector<ti::Variable*> variables) noexcept
                : variables(variables)
            {
                type = ti::StatementType::VARIABLE;
            }
        };
    }
}

#endif /* Statement_hpp */
