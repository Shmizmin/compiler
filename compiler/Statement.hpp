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
        VARIABLE,
    };
    
    
    struct Statement;
    
    namespace stmt
    {
        struct Block
        {
            std::vector<Statement*> statements;
        };
        
        struct If
        {
            Expression* condition;
            Statement* statement;
        };

        struct While
        {
            Expression* condition;
            Statement* statement;
        };
        
        struct Return
        {
            Expression* value;
        };
        
        struct Variable
        {
            std::vector<Variable*> variables;
        };
    }
    
    struct Statement
    {
        const StatementType type;
        
        const union
        {
            stmt::Block as_block;
            stmt::If as_if;
            stmt::While as_while;
            stmt::Return as_return;
            stmt::Variable as_variable;
        } value;
    };
}

#endif /* Statement_hpp */
