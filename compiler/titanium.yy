%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.8.1"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
#include <string>
  class driver;
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error verbose

%code {
#include "driver.hpp"
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
    BITXOR "^"

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
%left "^"

%left ">>" "<<"
%left "+" "-"

%right "++" "--"

%left "("



%token<std::string> IDENTIFIER

%token<std::string> STRINGCONST
%token<int> NUMCONST

%type<ti::Expression*> exp

%printer { yyoutput << $$; } <*>;

%%
    
%start translation_unit;

translation_unit
: definitions_opt END
;

// definitions
definitions_opt
: %empty
| definitions
;

definitions
: definitions definition
| definition
;

definition
: function_declarator
;
// /definitions


// types
type_specifier
: "byte"
| "void"
;

type_qualifier
: "val"
;

type_visibility
: "local"
| "global"
;

complete_type
: type_specifier type_qualifier
;
// /types


// functions
function_declarator
: "proto" function_header ";"
| function_header statement
;

function_header
: "function" complete_type IDENTIFIER "=" "(" fdecl_args_opt ")"
;

    // function definition args
    fdecl_args_opt
    : %empty
    | fdecl_args
    ;
    
    fdecl_args
    : complete_type IDENTIFIER
    | fdecl_args "," complete_type IDENTIFIER
    ;
    // /function definition args
    
    // function call args
    fcall_args_opt
    : %empty
    | fcall_args
    ;
    
    fcall_args
    : expression
    | fcall_args "," expression
    ;
    // /function call args
    
// /functions


// variables
variable_declarator
: type_visibility complete_type variable_declarator_i
| variable_declarator "," variable_declarator_i
;

variable_declarator_i
: IDENTIFIER "=" expression
| IDENTIFIER
;
// /variables


// errors
expect_semicolon
: error { /*Meaningful error message*/ }
| ";"
;

expect_rparen
: error { /*Meaningful error message*/ }
| ")"
;

expect_rbrace
: error { /*Meaningful error message*/ }
| "}"
;
// /errors


// statements
statement
: "{" statements_opt "}"
| "if" "(" expression ")" statement
| "while" "(" expression ")" statement
| "return" expression_opt ";"
| ";"
| variable_declarator ";"
;

statements
: statements statement
| statement
;

statements
: %empty
| statements
;
// /statements


// expressions
expression
: NUMCONST { $$ = new ti::expr::num}
| STRINGCONST
| IDENTIFIER
| "(" expression ")"
| T_IDENTIFIER "("  args_delim_opt expect_rparen
| T_IDENTIFIER "=" expression
| expression "+"  expression
| expression "-"  expression %prec "+"
| expression "<<" expression
| expression ">>" expression %prec "<<"
| expression "^"  expression
| expression "&"  expression
| expression "|"  expression
| T_IDENTIFIER "++"
| T_IDENTIFIER "--"          %prec "++"
| expression "==" expression
| expression "!=" expression %prec "=="
|            "+"  expression
|            "-"  expression %prec "+"
| expression "?"  expression ":" expression
| expression "<"  expression
| expression ">"  expression
;

expression_opt
: %empty
| expression
;
// /expressions


/*
 exp:
 ;  exp "+" exp   { $$ = $1 + $3; }
 | exp "-" exp   { $$ = $1 - $3; }
 | exp "*" exp   { $$ = $1 * $3; }
 | exp "/" exp   { $$ = $1 / $3; }
 | "(" exp ")"   { std::swap ($$, $2); }
 | "identifier"  { $$ = drv.variables[$1]; }
 | "number"      { std::swap ($$, $1); };
 */

%%

void yy::parser::error(const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}
