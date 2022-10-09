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
    
    
    struct Var
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
            std::vector<Var*> variables;
        };
    }
    
    struct Statement
    {
        const StatementType type;
        
        const union
        {
            stmt::Block block;
            stmt::If ifs;
            stmt::While whiles;
            stmt::Return returns;
            stmt::Variable variable;
        } as;
    };
    
    Statement* make_block(std::vector<Statement*>&&) noexcept;
    Statement* make_if(Expression*, Statement*) noexcept;
    Statement* make_while(Expression*, Statement*) noexcept;
    Statement* make_return(Expression*) noexcept;
    Statement* make_variable(std::vector<Variable*>&&) noexcept;
    
    void compile_statement(Statement*, CommonArgs&) noexcept;
}

#endif /* Statement_hpp */
