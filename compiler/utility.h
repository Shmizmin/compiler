#ifndef utility_h
#define utility_h

#include <stdlib.h>


typedef unsigned int size;
extern size line;

typedef enum
{
    BYTE,
    VOID,
} Type_Specifier;

typedef enum
{
    VAL,
} Type_Qualifier;

typedef enum
{
    LOCAL,
    GLOBAL,
} Type_Visibility;

typedef struct
{
    Type_Specifier type_specifier;
    Type_Qualifier type_qualifier;
    const char* ident;
} Complete_Type;


typedef struct
{
    Complete_Type complete_type;
} Function_Definition_Argument;

typedef struct
{
    Function_Definition_Argument* args;
    size_t num_args;
} Function_Definition_Arguments;

typedef struct
{
    Complete_Type complete_type;
    Function_Definition_Arguments arguments;
} Function_Prototype;

typedef struct
{
    Function_Prototype* definition;
} Function_Definition;

typedef enum
{
    Function,
} Definition_Type;

typedef union
{
    
} Definition_

typedef union
{
    Function_Definition function_definition;
} Definition;


struct Statement;
struct Expression;


typedef struct
{
    struct Expression* value;
    const char* ident;
} Variable_Declaration;

typedef struct
{
    Variable_Declaration* decls;
    size_t num_decls;
} Variable_Declarations;

typedef struct
{
    Type_Specifier type_specifier;
    Type_Qualifier type_qualifier;
    Variable_Declarations* decls;
} Variable_Definition;

typedef struct
{
    Type_Visibility type_visibility;
    Variable_Definition* definition;
} Qualified_Variable_Definition;


typedef struct
{
    struct Statement* body;
} Block_Statement;

typedef struct
{
    struct Expression* condition;
    struct Statement* body;
} If_Statement;

typedef struct
{
    struct Expression* condition;
    struct Statement* body;
} While_Statement;

typedef struct
{
    struct Expression* condition;
} Return_Statement;

typedef struct
{
    //
} Null_Statement;

typedef struct
{
    Variable_Declaration* vars;
} Variable_Statement;

typedef enum
{
    Block,
    If,
    While,
    Return,
    Null,
    Variable,
} Statement_Type;

typedef union
{
    Block_Statement* block_statement;
    If_Statement* if_statement;
    While_Statement* while_statement;
    Return_Statement* return_statement;
    Null_Statement* null_statement;
    Variable_Statement* variable_statement;
} Statement_Value;

struct Statement
{
    Statement_Type statement_type;
    Statement_Value statement_value;
};

typedef struct
{
    struct Statement* stmts;
    size_t num_stmts;
} Statements;


typedef struct
{
    Expression* exprs;
    size_t num_exprs;
} Expressions;

#ifdef __cplusplus
extern "C"
{
#endif
    
    char* capture(char* in, size n, size m);
    
    void begin_compiler(void);
    Definition* create_definition(Function_Definition*);
    
    Complete_Type make_complete_type(Type_Specifier, Type_Qualifier, const char*);

    Function_Definition* create_function_proto(Complete_Type, Function_Definition_Argument*);
    
    //for function definition parameter specification
    Function_Definition_Arguments* create_first_arg_decl(Complete_Type);
    Function_Definition_Arguments* create_later_arg_decl(Complete_Type);
    
    //function declarators
    Function_Definition* create_function_undef(Function_Prototype);
    Function_Definition* create_function_def(Function_Prototype);
    
    
    //full variable declaration/definition, just without visibility
    Variable_Definition* create_first_var_decl(Type_Specifier, Type_Qualifier, Variable_Declaration*);
    Variable_Definition* create_later_var_decl(Variable_Declaration*);
    
    Variable_Declaration* create_variable_def(const char*, Expression*);
    Variable_Declaration* create_variable_undef(const char*);
    
    //visibility specified variables
    Qualified_Variable_Definition* qualify_variable_definition(Variable_Definition*, Type_Visibility);
    
    
    Expressions* create_first_fcall_param(Expression*);
    Expressions* create_later_fcall_param(Expression*);
    
    
    Statements* append_statement_list(Statement*);
    Statements* return_statement_list(void);
    
    
       Block_Statement* create_stmt_block(Statements*);
          If_Statement* create_stmt_if(Expression*, Statement*);
       While_Statement* create_stmt_while(Expression*, Statement*);
      Return_Statement* create_stmt_return(Expression*);
        Null_Statement* create_stmt_null(void);
    Variable_Statement* create_stmt_variable(Qualified_Variable_Definition*);
    
    Expression* create_expr_numconst(uint8_t);
    Expression* create_expr_stringconst(const char*);
    Expression* create_expr_identifier(const char*);
    Expression* create_expr_fcall(const char*, Expressions*);
    Expression* create_expr_equals(const char*, Expression*);
    Expression* create_expr_plus(Expression*, Expression*);
    Expression* create_expr_minus(Expression*, Expression*);
    Expression* create_expr_lshift(Expression*, Expression*);
    Expression* create_expr_rshift(Expression*, Expression*);
    Expression* create_expr_xor(Expression*, Expression*);
    Expression* create_expr_and(Expression*, Expression*);
    Expression* create_expr_or(Expression*, Expression*);
    Expression* create_expr_inc(const char*);
    Expression* create_expr_dec(const char*);
    Expression* create_expr_isequal(Expression*, Expression*);
    Expression* create_expr_notequal(Expression*, Expression*);
    Expression* create_expr_positive(Expression*);
    Expression* create_expr_negative(Expression*);
    Expression* create_expr_ternary(Expression*, Expression*, Expression*);
    Expression* create_expr_less(Expression*, Expression*);
    Expression* create_expr_greater(Expression*, Expression*);
    
    void create_ast(void);
    void generate_ast(void);

#ifdef __cplusplus
}
#endif


#endif /* utility.h */
