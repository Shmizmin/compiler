%{
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include "utility.h"

#define YYDEBUG 1

#define EX yyerror("expected expression");

extern int yylex(void);
extern int yyparse(void);
extern FILE* yyin;

void yyerror(const char* s);
%}

%union
{
    uint8_t ival;
    char* cval;
}

%token T_END 0
%token T_MINUSMINUS "--"
       T_MINUS      "-"
       T_PLUSPLUS   "++"
       T_PLUS       "+"
       T_LNOT       "!"
       T_BOR        "|"
       T_BAND       "&"
       T_BXOR       "^"

       T_LSHIFT "<<"
       T_RSHIFT ">>"

       T_LESS    "<"
       T_GREATER ">"

       T_NOTEQ "!="
       T_EQEQ  "=="

       T_SEMICOLON ";"
       T_COMMA     ","
       T_EQ        "="
       T_LPAREN    "("
       T_RPAREN    ")"
       T_LBRACE    "{"
       T_RBRACE    "}"


%token T_FUNCTION "function"
       T_PROTO    "proto"
       T_PTR      "ptr"
       T_VAL      "val"
       T_VOID     "void"
       T_BYTE     "byte"
       T_LOCAL    "local"
       T_GLOBAL   "global"
       T_IF       "if"
       T_ELSE     "else"
       T_WHILE    "while"
       T_FOR      "for"
       T_RETURN   "return"
       T_ADDROF   "addrof"
       T_DEREF    "deref"


%left  ","
%right "?" ":" "="
%left  "==" "!="
%left  ">" "<"
%left  "|"
%left  "&"
%left  "^"
%left  "<<" ">>"
%left  "+" "-"
%right "addrof" "deref"
%right "~"
%right "++" "--"
%left  "("

%token<ival> T_NUMCONST
%token<cval> T_IDENTIFIER T_STRINGCONST

%start program

%expect 1

%%

program
    : definitions_opt T_END
    ;
    
definitions_opt
    : %empty
    | definitions_opt definition
    ;
    
definition
    : function_declarator
    | global_declarator
    ;
    
type_specifier
    : "byte"
    | "void"
    ;
    
type_qualifier
    : "ptr"
    | "val"
    ;
    
complete_type
    : type_specifier type_qualifier T_IDENTIFIER
    ;
    
function_prototype
    : "function" complete_type "=" "(" arguments_opt expect_rparen
    ;
    
arguments
    : complete_type
    | arguments_opt "," complete_type
    ;
    
arguments_opt
    : %empty
    | arguments
    ;
    
function_declarator
    : "proto" function_prototype expect_semicolon
    | function_prototype statement
    ;
    
variable_declarator
    : type_specifier type_qualifier variable_declarator_i
    | variable_declarator "," variable_declarator_i
    ;

variable_declarator_i
    : T_IDENTIFIER "=" expression
    | T_IDENTIFIER
    ;
    
local_declarator
    : "local" variable_declarator { $$ = $2; }
    ;
    
global_declarator
    : "global" variable_declarator { $$ = $2; }
    ;
    
expect_semicolon
    : error { yyerror("expected \';\'"); }
    | ";"
    ;
    
expect_rparen
    : error { yyerror("expected \')\'"); }
    | ")"
    ;
    
expect_rbrace
    : error { yyerror("expected \'}\'"); }
    | "}"
    ;
    
else_opt
    : %empty
    | "else" statement
    ;
    
args_delim
    : expression
    | args_delim "," expression
    ;
    
args_delim_opt
    : %empty
    | args_delim
    ;
    
statements
    : statements statement
    | %empty
    ;
    
statement
    : "{" statements "}"
    | "if" "(" expression ")" statement else_opt
    | "while" "(" expression ")" statement
    | "return" expression_opt ";"                   { create_stmt_return($2); }
    | expect_semicolon                              { create_stmt_null(); }
    | local_declarator ";"                          { create_stmt_localvar($1); }
    | global_declarator ";"                         { create_stmt_globalvar($1); }
    ;

expression_opt
    : %empty { $$ = Expression{}; }
    | expression { }
    ;
    
expression
    : T_NUMCONST                                     { create_expr_numconst($1); }
    | T_STRINGCONST                                  { create_expr_stringconst($1); }
    | T_IDENTIFIER                                   { create_expr_identifier($1); }
    | "(" expression ")"                             { create_expr_paren($2); }
    | T_IDENTIFIER "("  args_delim_opt expect_rparen { create_expr_fcall($1, $3); }//function call
    | T_IDENTIFIER "=" expression                    { create_expr_equals($1, $3); }
    | expression "+"  expression                     { create_expr_plus($1, $3); }
    | expression "-"  expression %prec "+"           { create_expr_minus($1, $3); }
    | expression "<<" expression                     { create_expr_lshift($1, $3); }
    | expression ">>" expression %prec "<<"          { create_expr_rshift($1, $3); }
    | expression "^"  expression                     { create_expr_xor($1, $3); }
    | expression "&"  expression                     { create_expr_and($1, $3); }
    | expression "|"  expression                     { create_expr_or($1, $3); }
    | T_IDENTIFIER "++"                              { create_expr_inc($1); }
    | T_IDENTIFIER "--"          %prec "++"          { create_expr_dec($1); }
    | expression "==" expression                     { create_expr_isequal($1, $3); }
    | expression "!=" expression %prec "=="          { create_expr_isnotequal($1, $3); }
    |            "+"  expression                     { create_expr_positive($2); }
    |            "-"  expression %prec "-"           { create_expr_negative($2); }
    | expression "?"  expression ":" expression      { create_expr_ternary($1, $3, $5); }
    | expression "<"  expression                     { create_expr_less($1, $3); }
    | expression ">"  expression                     { create_expr_greater($1, $3); }
    ;

%%

size line = 1;
    
int main(int argc, char** argv)
{
    //provide parser instrumentation
    yydebug = 1;
    
    
    create_ast();
    generate_ast();
    
    /*
    assert(argc > 1);
    yyin = fopen(argv[1], "r");
    assert(yyin != NULL);
    
    do
    {
        yyparse();
    } while(!feof(yyin));*/

    return 0;
}

void yyerror(const char* s)
{
    fprintf(stderr, "Parse error: %s on line %u\n", s, line);
    exit(1);
}

