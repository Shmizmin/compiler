%option nodefault noyywrap
%option nounput noinput
%option warn debug

%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define YY_DECL int yylex()

#include "y.tab.h"
#include "utility.h"

//(\'(\\.|[^\\"])\')  { yylval.cval = capture(yytext, 1, 1); return T_CHAR; }
%}

%%

[ \t\n] { /* skip most whitespace */ }
\n      { ++line; }
    
"--"       { return T_MINUSMINUS; }
"-"        { return T_MINUS;      }
"++"       { return T_PLUSPLUS;   }
"+"        { return T_PLUS;       }
"!"        { return T_LNOT;       }
"|"        { return T_BOR;        }
"&"        { return T_BAND;       }
"^"        { return T_BXOR;       }
"<<"       { return T_LSHIFT;     }
">>"       { return T_RSHIFT;     }

"<"        { return T_LESS;       }
">"        { return T_GREATER;    }

"!="       { return T_NOTEQ;      }
"=="       { return T_EQEQ;       }

";"        { return T_SEMICOLON;  }
","        { return T_COMMA;      }
"="        { return T_EQ;         }
"("        { return T_LPAREN;     }
")"        { return T_RPAREN;     }
"{"        { return T_LBRACE;     }
"}"        { return T_RBRACE;     }

"function" { return T_FUNCTION;   }
"proto"    { return T_PROTO;      }
"copy"     { return T_VAL;        }
"ptr"      { return T_PTR;        }
"val"      { return T_VAL;        }
"void"     { return T_VOID;       }
"byte"     { return T_BYTE;       }
"bool"     { return T_BYTE;       }
"local"    { return T_LOCAL;      }
"global"   { return T_GLOBAL;     }
"if"       { return T_IF;         }
"for"      { return T_FOR;        }
"while"    { return T_WHILE;      }
"return"   { return T_RETURN;     }
  
[0-9]+           { yylval.cval = capture(yytext, 0, 0); return T_NUMCONST; }
(0b[0-1]+)       { yylval.cval = capture(yytext, 2, 0); return T_NUMCONST; }
(0x[0-9a-fA-F]+) { yylval.cval = capture(yytext, 2, 0); return T_NUMCONST; }

(\/\/[^\r\n]*)                               { /* single line comment */ }
\/\*([^*]|[\r\n]|(\*+([^*\/]|[\r\n])))*\*+\/ { /*  multi line comment */ }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.cval = capture(yytext, 0, 0); return T_IDENTIFIER; }

(\"(\\.|[^\\"])*\") { yylval.cval = capture(yytext, 1, 1); return T_STRINGCONST; }


%%
