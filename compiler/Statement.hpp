#ifndef Statement_hpp
#define Statement_hpp

#include <vector>

#include "Expression.hpp"
#include "Types.hpp"

namespace ti
{
    
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
    };
    
    struct BlockStatement : public Statement
    {
        std::vector<Statement*> statements;
    };
    
    struct IfStatement : public Statement
    {
        Expression* condition;
        Statement* statement;
    };
    
    struct WhileStatement : public Statement
    {
        Expression* condition;
        Statement* statement;
    };
    
    struct ReturnStatement : public Statement
    {
        Expression* value;
    };
    
    struct NilStatement : public Statement
    {
        //null
    };
    
    struct VariableStatemnet : public Statement
    {
        std::vector<Variable> variables;
    };
}

#endif /* Statement_hpp */
