#ifndef codegen_h
#define codegen_h

//#include <stdint.h>
//#include <stdbool.h>


/*
 Structure:
 
 Translation Unit (only 1 at a time for now)
        Preprocessor Directives
        Functions->
            Statements->
                Expression
        

 
 
 */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;



typedef enum
{
    LOCAL,
    GLOBAL,
} TypeVisibility;

typedef enum
{
    BYTE,
    WORD,
    VOID,
} TypeSpecifier;

typedef enum
{
    COPY, //does not exist for function return types (throw error)
    PTR,
    VAL,
} TypeQualifier;

typedef struct
{
    TypeSpecifier type_specifier;
    TypeQualifier type_qualifier;
} CompleteType;




typedef enum : uint8_t
{
    PLUS_PLUS,
    MINUS_MINUS,
    
    ADDROF,
    DEREF,
    
    POSITIVE,
    NEGATIVE,
} UnaryOp;

typedef enum
{
    INDEX,
    FCALL,
    
    EQUALS,
    
    PLUS,
    MINUS,
    
    PLUS_EQUALS,
    MINUS_EQUALS,
    
    LEFT_SHIFT,
    RIGHT_SHIFT,
    
    BIT_XOR,
    BIT_AND,
    BIT_OR,
    
    LOG_AND,
    LOG_OR,
    
    EQUALS_EQUALS,
    NOT_EQUALS,
    
    LESS,
    GREATER,
    LESS_EQUALS,
    GREATER_EQUALS,
} BinaryOp;





struct NumconstExpression_t;
struct StringconstExpression_t;
struct IdentifierExpression_t;
struct TernaryExpression_t;
struct BinaryExpression_t;
struct UnaryExpression_t;



typedef enum
{
    NUMCONST,
    STRINGCONST,
    IDENTIFIER,
    TERNARYOP,
    BINARYOP,
    UNARYOP,
} ExpressionType;
typedef union
{
    struct NumconstExpression_t* numconst_expression;
    struct StringconstExpression_t* stringconst_expression;
    struct IdentifierExpression_t* identifier_expression;
    struct TernaryExpression_t* ternary_expression;
    struct BinaryExpression_t* binary_expression;
    struct UnaryExpression_t* unary_expression;
} ExpressionValue;
typedef struct
{
    ExpressionType expression_type;
    ExpressionValue expression_value;
} Expression;





typedef struct NumconstExpression_t
{
    uint16_t value;
} NumconstExpression;

typedef struct StringconstExpression_t
{
    const char* value;
} StringconstExpression;

typedef struct IdentifierExpression_t
{
    const char* name;
} IdentifierExpression;

typedef struct TernaryExpression_t
{
    Expression *left, *center, *right;
} TernaryExpression;

typedef struct BinaryExpression_t
{
    BinaryOp op;
    Expression *left, *right;
} BinaryExpression;

typedef struct UnaryExpression_t
{
    UnaryOp op;
    Expression* left;
} UnaryExpression;





typedef struct
{
    TypeVisibility visbility;
    CompleteType complete_type;
    const char* name;
    Expression* value; //null if value undefined
} Variable;

typedef struct
{
    uint8_t num_variables;
    Variable* variables;
} VariableStatement;


typedef enum
{
    BLOCK,
    IF,
    WHILE,
    RETURN,
    NIL,
    VARIABLE,
} StatementType;


struct BlockStatement_t;
struct IfStatement_t;
struct WhileStatement_t;
struct ReturnStatement_t;
struct NilStatement_t;


typedef union
{
    struct BlockStatement_t* block_statement;
    struct IfStatement_t* if_statement;
    struct WhileStatment_t* while_statement;
    struct ReturnStatement_t* return_statement;
    struct NilStatement_t* nil_statement;
    VariableStatement* variable_statement;
} StatementValue;
typedef struct
{
    StatementType statement_type;
    StatementValue statement_value;
} Statement;


struct BlockStatement_t
{
    uint8_t num_statements;
    Statement* statements;
};

struct IfStatement_t
{
    Expression* condition;
    Statement* statement;
};

struct WhileStatement_t
{
    Expression* condition;
    Statement* statement;
};

struct ReturnStatement_t
{
    Expression* value;
};

struct NilStatement_t
{
    //null
};







typedef struct
{
    const char* name;
    CompleteType complete_type;
} Argument;

typedef struct
{
    uint8_t num_arguments;
    Argument* arguments;
} Arguments;

typedef struct
{
    const char* name;
    CompleteType return_type; //void must be 'val' qualified
    Arguments arguments;
    Statement* body; //null if function body undefined
} Function;


typedef struct
{
    uint8_t num_functions;
    Function* functions;
} Program;






#ifdef __cplusplus
extern "C"
{
#endif
    
void generate(const Program* program);

#ifdef __cplusplus
}
#endif

#endif /* codegen_h */
