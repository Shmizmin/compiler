#ifndef utility_h
#define utility_h

#include <stdlib.h>


typedef unsigned int size;

extern size line;

#ifdef __cplusplus
extern "C"
{
#endif
    
    char* capture(char* in, size n, size m);
    
    void create_expr_numconst(uint8_t);
    void create_expr_stringconst(char*);
    void create_expr_identifier(char*);
    void create_expr_paren(Expression*);
    void create_expr_fcall(char*, Arguments*);
    void create_expr_equals(char*, Expression*);
    void create_expr_plus(Expression*, Expression*);
    void create_expr_minus(Expression*, Expression*);
    
    void create_expr_lshift(Expression*, Expression*);
    void create_expr_rshift(Expression*, Expression*);
    
    void create_expr_xor(Expression*, Expression*);
    void create_expr_and(Expression*, Expression*);
    void create_expr_or(Expression*, Expression*);
    
    void create_expr_inc(char*);
    void create_expr_dec(char*);
    
    void create_expr_isequal(Expression*, Expression*);
    void create_expr_isnotequal(Expresion*, Expression*);
    
    void create_expr_positive(Expression*);
    void create_expr_negative(Expression*);
    
    void create_expr_ternary(Expression*, Expression*, Expression*);
    
    void create_expr_less(Expression*, Expression*);
    void create_expr_greater(Expression*, Expression*);
    
    
    
    void create_ast(void);
    void generate_ast(void);

#ifdef __cplusplus
}
#endif


#endif /* utility.h */
