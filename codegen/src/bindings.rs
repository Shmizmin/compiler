#![allow(non_camel_case_types)]

use std::os::raw::c_char;
use std::os::raw::c_uchar;
use std::os::raw::c_ushort;

#[repr(C)]
#[derive(Copy, Clone)]
pub struct Parameters
{
    silence_warnings: bool,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub enum TypeVisibility
{
    LOCAL,
    GLOBAL,
} 

#[repr(C)]
#[derive(Copy, Clone)]
pub enum TypeSpecifier
{
    BYTE, 
    WORD,
    VOID,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub enum TypeQualifier
{
    COPY,
    PTR,
    VAL,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct CompleteType
{
    pub type_specifier: TypeSpecifier,
    pub type_qualifier: TypeQualifier,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub enum UnaryOp
{
    PLUS_PLUS,
    MINUS_MINUS,
    
    ADDROF,
    DEREF,

    POSITIVE,
    NEGATIVE,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub enum BinaryOp
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
}

#[repr(C)]
#[derive(Copy, Clone)]
pub enum ExpressionType
{
    NUMCONST,
    STRINGCONST,
    IDENTIFIER,
    TERNARYOP,
    BINARYOP,
    UNARYOP,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub union ExpressionValue
{
    pub numconst_expression: *mut NumconstExpression,
    pub stringconst_expression: *mut StringconstExpression,
    pub identifier_expression: *mut IdentifierExpression,
    pub ternary_expression: *mut TernaryExpression,
    pub binary_expression: *mut BinaryExpression,
    pub unary_expression: *mut UnaryExpression,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct Expression
{
    pub expression_type: ExpressionType,
    pub expression_value: ExpressionValue,
}


#[repr(C)]
#[derive(Copy, Clone)]
pub struct NumconstExpression
{
    pub value: c_ushort,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct StringconstExpression
{
    pub value: *const c_char,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct IdentifierExpression
{
    pub name: *const c_char,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct TernaryExpression
{
    pub left: *mut Expression,
    pub center: *mut Expression,
    pub right: *mut Expression,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct BinaryExpression
{
    pub op: BinaryOp,
    pub left: *mut Expression,
    pub right: *mut Expression,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct UnaryExpression
{
    pub op: UnaryOp,
    pub left: *mut Expression,
}


#[repr(C)]
#[derive(Copy, Clone)]
pub struct Variable
{
    pub visibility: TypeVisibility,
    pub complete_type: CompleteType,
    pub name: *const c_char,
    pub value: *mut Expression, //null if value undefined
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct VariableStatement
{
    pub num_variables: c_uchar,
    pub variables: *mut Variable,
}


#[repr(C)]
#[derive(Copy, Clone)]
pub enum StatementType
{
    BLOCK,
    IF,
    WHILE,
    RETURN,
    NIL,
    VARIABLE,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub union StatementValue
{
    pub block_statement: *mut BlockStatement,
    pub if_statement: *mut IfStatement,
    pub while_statement: *mut WhileStatement,
    pub return_statement: *mut ReturnStatement,
    pub nil_statement: *mut NilStatement,
    pub variable_statement: *mut VariableStatement,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct Statement
{
    pub statement_type: StatementType,
    pub statement_value: StatementValue,
}


#[repr(C)]
#[derive(Copy, Clone)]
pub struct BlockStatement
{
    pub num_statements: c_uchar,
    pub statements: *mut Statement,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct IfStatement
{
    pub condition: *mut Expression,
    pub statement: *mut Statement,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct WhileStatement
{
    pub condition: *mut Expression,
    pub statement: *mut Statement,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct ReturnStatement
{
    pub value: *mut Expression,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct NilStatement
{
    //null
}


#[repr(C)]
#[derive(Copy, Clone)]
pub struct Argument
{
    pub name: *const c_char,
    pub complete_type: CompleteType,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct Arguments
{
    pub num_arguments: c_uchar,
    pub arguments: *mut Argument,
}


#[repr(C)]
#[derive(Copy, Clone)]
pub struct Function
{
    pub name: *const c_char,
    pub return_type: CompleteType, //void must be 'val' qualified
    pub arguments: Arguments,
    pub body: *mut Statement, //null if function body undefined
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct Program
{
    pub num_functions: c_uchar,
    pub functions: *mut Function,
}
