#include "utility.h"

#include <vector>
#include <string>
#include <cstring>

char* capture(char* in, size n, size m)
{
    std::string temp = in;
    temp = temp.substr(n, temp.length() - m);

    return strdup(temp.data());
}

#include "Context.hpp"
#include "Driver.hpp"
#include "Statement.hpp"
#include "Expression.hpp"

ti::Context context{};
ti::Program program{};


void create_ast(void)
{
    ti::Function fn1;
    fn1.name = "main";
    fn1.arguments = {};
    
    ti::Function fn2;
    fn2.name = "second";
    fn2.arguments = {};
    
    ti::CompleteType rt2{};
    rt2.specifier = ti::TypeSpecifier::BYTE;
    rt2.qualifier = ti::TypeQualifier::VAL;
    
    fn2.return_type = rt2;
    
    auto* body = new ti::stmt::Block();
    body->type = ti::StatementType::BLOCK;
    
    auto* rx = new ti::stmt::Return();
    rx->type = ti::StatementType::RETURN;
    
    auto* rvx = new ti::expr::Numconst();
    rvx->type = ti::ExpressionType::NUMCONST;
    rvx->value = 1;
    
    rx->value = rvx;
    
    
    body->statements.emplace_back(rx);
    fn2.body = body;
    
    
    
    
    ti::CompleteType rt{};
    rt.specifier = ti::TypeSpecifier::VOID;
    rt.qualifier = ti::TypeQualifier::VAL;
    
    fn1.return_type = rt;
    
    auto* stmt = new ti::stmt::Block();
    stmt->type = ti::StatementType::BLOCK;
    
    auto* vars = new ti::stmt::Variable();
    vars->type = ti::StatementType::VARIABLE;
    
    auto* var = new ti::Variable();
    var->type.qualifier = ti::TypeQualifier::VAL;
    var->type.specifier = ti::TypeSpecifier::BYTE;
    
    var->name = "res";
    var->visibility = ti::TypeVisibility::LOCAL;
    
    auto* bine = new ti::expr::binary::Plus();
    bine->type = ti::ExpressionType::BINARYOP;
    
    auto* bl = new ti::expr::Numconst();
    bl->type = ti::ExpressionType::NUMCONST;
    bl->value = 1;
    
    
    auto* br2 = new ti::expr::FCall();
    br2->type = ti::ExpressionType::BINARYOP;
    
    auto* ident = new ti::expr::Identifier();
    ident->type = ti::ExpressionType::IDENTIFIER;
    ident->name = "second";
    
    br2->left = ident;
    br2->args = {};
    
    bine->left = bl;
    bine->right = br2;
    
    var->value = bine;
    
    vars->variables.emplace_back(var);
    
    
    stmt->statements.emplace_back(vars);
    
    auto* ret = new ti::stmt::Return();
    ret->type = ti::StatementType::RETURN;
    
    auto* neg = new ti::expr::unary::Negative();
    neg->type = ti::ExpressionType::UNARYOP;
    
    auto* id = new ti::expr::Identifier();
    id->type = ti::ExpressionType::IDENTIFIER;
    id->name = "res";
    
    neg->center = id;
    
    ret->value = neg;
    
    stmt->statements.emplace_back(ret);
    
    
    
    
    fn1.body = stmt;
                                  
    program.functions.emplace_back(fn2);
    program.functions.emplace_back(fn1);
    
    //program.functions[0]->
}

void generate_ast(void)
{
    ti::Parameters parameters{};
    parameters.file_name = "test.ti";
    
    ti::generate_program(program, parameters);
}
