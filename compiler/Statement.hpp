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
        
        struct Variables
        {
            std::vector<Variable*> variables;
        };
    }
    
    struct Statement
    {
        StatementType type;
        
        union As
        {
            stmt::Block block;
            stmt::If ifs;
            stmt::While whiles;
            stmt::Return returns;
            stmt::Variables variable;
            
            As(void) noexcept {}
            ~As(void) noexcept {}
        } as;
    };
    
    Statement* make_block(std::vector<Statement*>&&) noexcept;
    Statement* make_if(Expression*, Statement*) noexcept;
    Statement* make_while(Expression*, Statement*) noexcept;
    Statement* make_return(Expression*) noexcept;
    Statement* make_variable(std::vector<Variable*>&&) noexcept;
    
    void compile_statement(Statement*, CommonArgs&) noexcept;
}

#endif
