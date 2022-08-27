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
    
    ti::CompleteType rt{};
    rt.specifier = ti::TypeSpecifier::VOID;
    rt.qualifier = ti::TypeQualifier::VAL;
    
    fn1.return_type = rt;
    
    auto* stmt = new ti::stmt::Block();
    stmt->type = ti::StatementType::BLOCK;
    
    auto* vars = new ti::stmt::Variable();
    vars->type = ti::StatementType::VARIABLE;
    
    ti::Variable var{};
    var.type.qualifier = ti::TypeQualifier::VAL;
    var.type.specifier = ti::TypeSpecifier::BYTE;
    
    var.name = "res";
    var.visibility = ti::TypeVisibility::LOCAL;
    
    auto* bine = new ti::expr::binary::Plus();
    bine->type = ti::ExpressionType::BINARYOP;
    
    auto* bl = new ti::expr::Numconst();
    bl->type = ti::ExpressionType::NUMCONST;
    bl->value = 1;
    
    auto* br = new ti::expr::Numconst();
    br->type = ti::ExpressionType::NUMCONST;
    br->value = 2;
    
    bine->left = bl;
    bine->right = br;
    
    var.value = bine;
    
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
                                  
    program.functions.emplace_back(fn1);
    
    //program.functions[0]->
}

void generate_ast(void)
{
    ti::Parameters parameters{};
    parameters.file_name = "test.ti";
    
    ti::generate_program(program, parameters);
}
