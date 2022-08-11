%{
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include "common.h"
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
    int ival;
    char* cval;
}

%token T_END 0
%token T_MINUSMINUS "--" T_MINUS "-" T_PLUSPLUS "++" T_PLUS "+" T_MULTIPLY "*" T_DIVIDE "/" T_PERCENT "%" T_LNOT "!" T_COMPL "~" T_LOR "||" T_BOR "|" T_LAND "&&" T_BAND "&" T_LXOR "^^" T_BXOR "^" T_LSHIFT "<<" T_RSHIFT ">>" T_LESSEQ "<=" T_GREATEREQ ">=" T_LESS "<" T_GREATER ">" T_NOTEQ "!=" T_EQEQ "==" T_SEMICOLON ";" T_COMMA "," T_EQ "=" T_LPAREN "(" T_RPAREN ")" T_LBRACE "{" T_RBRACE "}" T_LBRACKET "[" T_RBRACKET "]"

%token T_FUNCTION "function" T_PROTO "proto" T_COPY "copy" T_PTR "ptr" T_VAL "val" T_VOID "void" T_BYTE "byte" T_WORD "word" T_LOCAL "local" T_GLOBAL "global" T_IF "if" T_ELSE "else" T_WHILE "while" T_FOR "for" T_RETURN "return" T_ADDROF "addrof" T_DEREF "deref"


%left  ","
%right "?" ":" "=" "+=" "-="
%left  "||"
%left  "&&"
%left  "==" "!="
%left  ">" ">=" "<" "<="
%left  "|"
%left  "&"
%left  "^"
%left  "<<" ">>"
%left  "+" "-"
%left  "*"
%right "&" "++" "--"
%left  "(" "["

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
    | "word"
    | "void"
    ;
    
type_qualifier
    : "copy"
    | "ptr"
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
    : "proto" function_prototype
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
    : "local" variable_declarator
    ;
    
global_declarator
    : "global" variable_declarator
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
    
expect_rbracket
    : error { yyerror("expected \']\'"); }
    | "]"
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
    | "return" expression ";"
    | expect_semicolon
    | local_declarator ";"
    | global_declarator ";"
    ;

expression
    : T_NUMCONST
    | T_STRINGCONST
    | T_IDENTIFIER
    | "(" expression ")"
    | expression "["  expression "]"
    | expression "("  args_delim_opt ")"
    | expression "="  error{ EX } | expression "="  expression
    | expression "+"  error{ EX } | expression "+"  expression
    | expression "-"  error{ EX } | expression "-"  expression %prec "+"
    | expression "+=" error{ EX } | expression "+=" expression
    | expression "-=" error{ EX } | expression "-=" expression
    | expression "<<" error{ EX } | expression "<<" expression
    | expression ">>" error{ EX } | expression ">>" expression
    | expression "^"  error{ EX } | expression "^"  expression
    | expression "&"  error{ EX } | expression "&"  expression
    | expression "|"  error{ EX } | expression "|"  expression
    |            "++" error{ EX } |            "++" expression
    |            "--" error{ EX } |            "--" expression %prec "++"
    |                               expression "++"
    |                               expression "--"            %prec "++"
    | expression "||" error{ EX } | expression "||" expression
    | expression "&&" error{ EX } | expression "&&" expression
    | expression "==" error{ EX } | expression "==" expression
    | expression "!=" error{ EX } | expression "!=" expression %prec "=="
    |            "&"  error{ EX } |            "&"  expression
    |            "*"  error{ EX } |            "*"  expression %prec "&"
    |            "-"  error{ EX } |            "-"  expression %prec "&"
    |            "+"  error{ EX } |            "+"  expression %prec "&"
    | expression "?"  error{ EX } | expression "?"  expression ":" expression
    | expression "<"  error{ EX } | expression "<"  expression
    | expression ">"  error{ EX } | expression ">"  expression
    | expression "<=" error{ EX } | expression "<=" expression
    | expression ">=" error{ EX } | expression ">=" expression
    ;

%%

size line = 1;
    
int main(int argc, char** argv)
{
    //provide parser instrumentation
    yydebug = 1;
    
    
    assert(argc > 1);
    yyin = fopen(argv[1], "r");
    assert(yyin != NULL);
    
    do
    {
        yyparse();
    } while(!feof(yyin));

    return 0;
}

void yyerror(const char* s)
{
    fprintf(stderr, "Parse error: %s on line %u\n", s, line);
    exit(1);
}

