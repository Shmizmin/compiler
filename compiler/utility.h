#ifndef utility_h
#define utility_h

#include <stdlib.h>


typedef unsigned int size;
extern size line;

typedef enum
{
    BYTE,
    VOID,
    
    PTR,
    VAL,
} Types;

typedef enum
{
    LOCAL,
    GLOBAL,
} Access;



#ifdef __cplusplus
extern "C"
{
#endif
    
    char* capture(char* in, size n, size m);
    
    void begin_compiler(void);
    Definition create_definition(Function*);
    
    Complete_Type make_complete_type(Type_Specifier, TypeQualifier, const char*);
    
    Function create_function_proto(Complete_Type, Argument*);
    
    Argument* create_first_arg_decl(CompleteType);
    Argument* create_later_arg_decl(CompleteType);
    
    Function create_function_undef(Function_Proto);
    Function create_function_def(Function_Proto);
    
    Variable* create_first_var_decl(Type_Specifier, Type_Qualifier, Var_Decl);
    Variable* create_later_var_decl(Var_Decl);
    
    Var_Decl create_variable_def(const char*, Expression*);
    Var_Decl create_variable_undef(const char*);
    
    
    Local_Decl create_local_variable_decl(Var_Decl);
    Global_Decl create_global_variable_decl(Var_Decl);
    
    FCall_Arg create_first_fcall_param(Expression*);
    FCall_Arg create_later_fcall_param(Expression*);
    
    
    
       Block_Statement* create_stmt_block(Statements*);
          If_Statement* create_stmt_if(Expression*, Statement*);
       While_Statement* create_stmt_while(Expression*, Statement*);
      Return_Statement* create_stmt_return(Expression*);
        Null_Statement* create_stmt_null(void);
    Variable_Statement* create_stmt_variable(Var_Decl*);
    
    
    Expression* create_expr_numconst(uint8_t);
    Expression* create_expr_stringconst(const char*);
    Expression* create_expr_identifier(const char*);
    Expression* create_expr_fcall(const char*, FCall_Args*);
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
    Expression* create_expr_notequal(Expression*, Expresion*);
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
