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
    uint8_t int_value;
    char* char_value;
    
    Function_Definition_Arguments* fdef_args_value;
    Function_Definition* fdef_value;
    Definition* definition_value;
    Type_Specifier type_specifier_value;
    Type_Qualifier type_qualifier_value;
    Complete_Type complete_type_value;
    Function_Prototype* fproto_value;
    
    Variable_Definition* vdef_value;
    Variable_Declaration* vdec_value;
    
    Qualified_Variable_Definition* qvar_defs_value;
    
    struct Statement* statement_value;
    Statements* statements_value;
    
    struct Expression* expression_value;
    Expressions* expressions_value;
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

%token<int_value> T_NUMCONST
%token<char_value> T_IDENTIFIER T_STRINGCONST

%type<definition_value> definitions_opt
%type<type_specifier_value> type_specifier
%type<type_qualifier_value> type_qualifier
%type<definition_value> definition
%type<fdef_value> function_declarator
%type<complete_type_value> complete_type
%type<fproto_value> function_prototype
%type<fdef_args_value> arguments_opt
%type<fdef_args_value> arguments
%type<statement_value> statement
%type<vdef_value> variable_declarator
%type<vdec_value> variable_declarator_i //internal
%type<expression_value> expression
%type<qvar_defs_value> local_declarator global_declarator
%type<expressions_value> args_delim args_delim_opt
%type<statements_value> statements
%type<expression_value> expression_opt

%start program

//%expect 1

%%

program
    : definitions_opt T_END { begin_compiler(); }
    ;
    
definitions_opt
    : %empty                     { $$ = Empty_Definition(); }
    | definitions_opt definition { $$ = $2; }
    ;
    
definition
    : function_declarator { $$ = make_definition(Function, $1); }
    ;
    
type_specifier
    : "byte" { $$ = BYTE; }
    | "void" { $$ = VOID; }
    ;
    
type_qualifier
    : "val" { $$ = VAL; }
    ;
    
complete_type
    : type_specifier type_qualifier T_IDENTIFIER { $$ = make_complete_type($1, $2, $3); }
    ;
    
function_prototype
    : "function" complete_type "=" "(" arguments_opt expect_rparen { $$ = create_function_proto($2, $5); }
    ;
    
arguments
    : complete_type                   { $$ = create_first_arg_decl($1); }
    | arguments_opt "," complete_type { $$ = create_later_arg_decl($3); }
    ;
    
arguments_opt
    : %empty    { $$ = NULL; }
    | arguments { $$ = $1; }
    ;
    
function_declarator
    : "proto" function_prototype expect_semicolon  { $$ = create_function_undef($2); }
    | function_prototype statement                 { $$ = create_function_def($1, $2); }
    ;
    
variable_declarator
    : type_specifier type_qualifier variable_declarator_i { $$ = create_first_var_decl($1, $2, $3); }
    | variable_declarator "," variable_declarator_i       { $$ = create_later_var_decl($3); }
    ;

variable_declarator_i
    : T_IDENTIFIER "=" expression { $$ = create_variable_def($1, $3); }
    | T_IDENTIFIER                { $$ = create_variable_undef($1); }
    ;
    
local_declarator
    : "local" variable_declarator { $$ = qualify_variable_definition($2, LOCAL); }
    |         variable_declarator { $$ = qualify_variable_definition($1, LOCAL); } //default local
    ;
    
global_declarator
    : "global" variable_declarator { $$ = qualify_variable_definition($2, GLOBAL); }
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
    
args_delim
    : expression                { $$ = create_first_fcall_param($1); }
    | args_delim "," expression { $$ = create_later_fcall_param($3); }
    ;
    
args_delim_opt
    : %empty     { $$ = Empty_Expressions(); }
    | args_delim { $$ = $1; }
    ;
    
statements
    : statements statement { $$ = append_statement_list($2); }
    | %empty               { $$ = return_statement_list(); }
    ;
    
statement
    : "{" statements "}"                   { $$ = create_stmt_block($2); }
    | "if" "(" expression ")" statement    { $$ = create_stmt_if($3, $5); }
    | "while" "(" expression ")" statement { $$ = create_stmt_while($3, $5); }
    | "return" expression_opt ";"          { $$ = create_stmt_return($2); }
    | expect_semicolon                     { $$ = create_stmt_null(); }
    | local_declarator ";"                 { $$ = create_stmt_variable($1); }
    | global_declarator ";"                { $$ = create_stmt_variable($1); }
    ;

expression_opt
    : %empty     { $$ = Empty_Expression(); }
    | expression { $$ = $1; }
    ;
    
expression
    : T_NUMCONST                                     { $$ = create_expr_numconst($1); }
    | T_STRINGCONST                                  { $$ = create_expr_stringconst($1); }
    | T_IDENTIFIER                                   { $$ = create_expr_identifier($1); }
    | "(" expression ")"                             { $$ = $2; }
    | T_IDENTIFIER "("  args_delim_opt expect_rparen { $$ = create_expr_fcall($1, $3); }//function call
    | T_IDENTIFIER "=" expression                    { $$ = create_expr_equals($1, $3); }
    | expression "+"  expression                     { $$ = create_expr_plus($1, $3); }
    | expression "-"  expression %prec "+"           { $$ = create_expr_minus($1, $3); }
    | expression "<<" expression                     { $$ = create_expr_lshift($1, $3); }
    | expression ">>" expression %prec "<<"          { $$ = create_expr_rshift($1, $3); }
    | expression "^"  expression                     { $$ = create_expr_xor($1, $3); }
    | expression "&"  expression                     { $$ = create_expr_and($1, $3); }
    | expression "|"  expression                     { $$ = create_expr_or($1, $3); }
    | T_IDENTIFIER "++"                              { $$ = create_expr_inc($1); }
    | T_IDENTIFIER "--"          %prec "++"          { $$ = create_expr_dec($1); }
    | expression "==" expression                     { $$ = create_expr_isequal($1, $3); }
    | expression "!=" expression %prec "=="          { $$ = create_expr_notequal($1, $3); }
    |            "+"  expression                     { $$ = create_expr_positive($2); }
    |            "-"  expression %prec "-"           { $$ = create_expr_negative($2); }
    | expression "?"  expression ":" expression      { $$ = create_expr_ternary($1, $3, $5); }
    | expression "<"  expression                     { $$ = create_expr_less($1, $3); }
    | expression ">"  expression                     { $$ = create_expr_greater($1, $3); }
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

