%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.8.2"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires
{
#include <string>
#include <vector>
#include <cstdint>
  class driver;
  
#include "function.hpp"
#include "statement.hpp"
#include "expression.hpp"
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error verbose
%define parse.lac full

%code
{
#include "types.hpp"
#include "driver.hpp"
#include "compiler.hpp"
#include "central.hpp"
}

%define api.token.prefix {T_}
%token
    END 0 "end of file"
    
    MINUS_MINUS "--"
    PLUS_PLUS "++"

    MINUS "-"
    PLUS "+"

    BITOR "|"
    BITAND "&"

    BITLSHIFT "<<"
    BITRSHIFT ">>"

    LESS "<"
    GREATER ">"

    NOT_EQ "!="
    IS_EQ "=="

    SEMICOLON ";"
    COMMA ","
    EQ "="
    LPAREN "("
    RPAREN ")"
    LBRACE "{"
    RBRACE "}"


    FUNCTION "function"
    PROTO "proto"
    VAL "val"
    VOID "void"
    BYTE "byte"
    LOCAL "local"
    GLOBAL "global"
    IF "if"
    WHILE "while"
    RETURN "return"
;

%left ","
%right "?" ":" "="

%left "==" "!="
%left ">" "<"
%left "|"
%left "&"

%left ">>" "<<"
%left "+" "-"

%right "++" "--"

%left "("



%token<std::string> IDENTIFIER

%token<std::string> STRINGCONST
%token<std::uint8_t> NUMCONST

%type<ti::Expression*> expression expression_opt
%type<ti::Statement*> statement

%type<std::vector<ti::Variable*>> variable_declarator
%type<ti::Variable*> variable_declarator_i

%type<ti::TypeVisibility> type_visibility
%type<ti::TypeSpecifier> type_specifier
%type<ti::TypeQualifier> type_qualifier
%type<ti::CompleteType> complete_type


%type<std::vector<ti::Statement*>> statements statements_opt

%type<ti::Function*> definition function_declarator function_header
%type<std::vector<ti::Function*>> definitions definitions_opt

%type<std::vector<ti::Argument>> fdecl_args fdecl_args_opt

%type<std::vector<ti::Expression*>> fcall_args fcall_args_opt
 
%%
    
%start translation_unit;

translation_unit
: definitions_opt END
{
    auto program = ti::Program{ $1 };
    auto parameters = ti::Parameters{ drv.file };
    
    ti::compile_program(program, parameters);
}
;

// definitions
definitions_opt
: %empty      { $$ = {}; }
| definitions { $$ = $1; }
;

definitions
: definitions definition
{
    auto& vec = $1;
    vec.emplace_back($2);
    $$ = vec;
}
| definition
{
    $$ = {};
    $$.emplace_back($1);
}
;

definition
: function_declarator { $$ = $1; }
;
// /definitions


// types
type_specifier
: "byte" { $$ = ti::TypeSpecifier::BYTE; }
| "void" { $$ = ti::TypeSpecifier::VOID; }
;

type_qualifier
: "val" { $$ = ti::TypeQualifier::VAL; }
;

type_visibility
: "local"  { $$ = ti::TypeVisibility::LOCAL; }
| "global" { $$ = ti::TypeVisibility::GLOBAL; }
;

complete_type
: type_specifier type_qualifier { $$ = ti::CompleteType{ $1, $2 }; }
;
// /types


// functions
function_declarator
: "proto" function_header ";"
{
    $$ = $2;
}
| function_header statement
{
    auto header = $1;
    header->body = $2;
    $$ = header;
}
;

function_header
: "function" complete_type IDENTIFIER "=" "(" fdecl_args_opt ")"
{
    $$ = new ti::Function{ $3, $2, $6, nullptr };
}
;

    // function definition args
    fdecl_args_opt
    : %empty     { $$ = {}; }
    | fdecl_args { $$ = $1; }
    ;
    
    fdecl_args
    : complete_type IDENTIFIER
    {
        $$ = std::vector<ti::Argument>();
        $$.emplace_back(ti::Argument{ $2, $1 });
    }
    | fdecl_args "," complete_type IDENTIFIER
    {
        auto& vec = $1;
        vec.emplace_back(ti::Argument{ $4, $3 });
        $$ = vec;
    }
    ;
    // /function definition args
    
    // function call args
    fcall_args_opt
    : %empty     { $$ = {}; }
    | fcall_args { $$ = $1; }
    ;
    
    fcall_args
    : expression
    {
        $$ = std::vector<ti::Expression*>();
        $$.emplace_back($1);
    }
    | fcall_args "," expression
    {
        auto& vec = $1;
        vec.emplace_back($3);
        $$ = vec;
    }
    ;
    // /function call args
    
// /functions


// variables
variable_declarator
: variable_declarator_i
{
    $$ = std::vector<ti::Variable*>();
    $$.emplace_back($1);
}
| variable_declarator "," variable_declarator_i
{
    auto& vec = $1;
    vec.emplace_back($3);
    $$ = vec;
}
;

variable_declarator_i
: type_visibility complete_type IDENTIFIER "=" expression
{
    $$ = new ti::Variable{ $1, $2, $3, $5 };
}
| type_visibility complete_type IDENTIFIER
{
    $$ = new ti::Variable{ $1, $2, $3, nullptr };
}
;
// /variables

// statements
statement
: "{" statements_opt "}"               { $$ = ti::make_block(std::move($2)); }
| "if" "(" expression ")" statement    { $$ = ti::make_if($3, $5); }
| "while" "(" expression ")" statement { $$ = ti::make_while($3, $5); }
| "return" expression_opt ";"          { $$ = ti::make_return($2); }
| ";"                                  { $$ = nullptr; }
| variable_declarator ";"              { $$ = ti::make_variable(std::move($1)); }
;

statements
: statements statement
{
    auto& vec = $1;
    vec.emplace_back($2);
    $$ = vec;
}
| statement
{
    $$ = std::vector<ti::Statement*>();
    $$.emplace_back($1);
}
;

statements_opt
: %empty     { $$ = {}; }
| statements { $$ = $1; }
;
// /statements


// expressions
expression
: "(" expression ")"                        { $$ = $2; }
| NUMCONST                                  { $$ = ti::make_numconst($1); }
| STRINGCONST                               { $$ = ti::make_stringconst(std::move($1)); }
| IDENTIFIER                                { $$ = ti::make_identifier(std::move($1)); }
| IDENTIFIER "("  fcall_args_opt ")"        { $$ = ti::make_functioncall(std::move($1), std::move($3)); }
|            "+"  expression                { $$ = $2; }
|            "-"  expression %prec "+"      { $$ = ti::make_unaryop($2, ti::UnaryOperator::NEGATIVE); }
| expression "++"                           { $$ = ti::make_unaryop($1, ti::UnaryOperator::PLUS_PLUS); }
| expression "--"            %prec "++"     { $$ = ti::make_unaryop($1, ti::UnaryOperator::MINUS_MINUS); }
| IDENTIFIER "="  expression                { $$ = ti::make_binaryop(ti::make_identifier(std::move($1)), $3, ti::BinaryOperator::EQUALS); }
| expression "+"  expression                { $$ = ti::make_binaryop($1, $3, ti::BinaryOperator::PLUS); }
| expression "-"  expression %prec "+"      { $$ = ti::make_binaryop($1, $3, ti::BinaryOperator::MINUS); }
| expression "<<" expression                { $$ = ti::make_binaryop($1, $3, ti::BinaryOperator::LEFT_SHIFT); }
| expression ">>" expression %prec "<<"     { $$ = ti::make_binaryop($1, $3, ti::BinaryOperator::RIGHT_SHIFT); }
| expression "&"  expression                { $$ = ti::make_binaryop($1, $3, ti::BinaryOperator::BIT_AND); }
| expression "|"  expression                { $$ = ti::make_binaryop($1, $3, ti::BinaryOperator::BIT_OR); }
| expression "==" expression                { $$ = ti::make_binaryop($1, $3, ti::BinaryOperator::EQUALS_EQUALS); }
| expression "!=" expression %prec "=="     { $$ = ti::make_binaryop($1, $3, ti::BinaryOperator::NOT_EQUALS); }
| expression "<"  expression                { $$ = ti::make_binaryop($1, $3, ti::BinaryOperator::LESS); }
| expression ">"  expression                { $$ = ti::make_binaryop($1, $3, ti::BinaryOperator::GREATER); }
| expression "?"  expression ":" expression { $$ = ti::make_ternaryop($1, $3, $5); }
;

expression_opt
: %empty     { $$ = nullptr; }
| expression { $$ = $1; }
;
// /expressions

%%

void yy::parser::error(const location_type& l, const std::string& m)
{
    std::cerr << "[Error] " << l << ": " << m << '\n';
}
