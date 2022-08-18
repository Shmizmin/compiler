#![allow(non_camel_case_types)]

use std::ffi::CStr;
use std::os::raw::c_char;

#[repr(C)]
pub enum TypeVisibility
{
    LOCAL,
    GLOBAL,
} 

#[repr(C)]
pub enum TypeSpecifier
{
    BYTE, 
    WORD,
    VOID,
}

#[repr(C)]
pub enum TypeQualifier
{
    COPY,
    PTR,
    VAL,
}

#[repr(C)]
pub struct CompleteType
{
    type_specifier: TypeSpecifier,
    type_qualifier: TypeQualifier,
}

#[repr(C)]
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
pub struct Expression
{
    pub expression_type: ExpressionType,
    pub expression_value: ExpressionValue,
}


#[repr(C)]
pub struct NumconstExpression
{
    pub value: u16,
}

#[repr(C)]
pub struct StringconstExpression
{
    pub value: *const c_char,
}

#[repr(C)]
pub struct IdentifierExpression
{
    pub name: *const c_char,
}

#[repr(C)]
pub struct TernaryExpression
{
    pub left: *mut Expression,
    pub center: *mut Expression,
    pub right: *mut Expression,
}

#[repr(C)]
pub struct BinaryExpression
{
    pub op: BinaryOp,
    pub left: *mut Expression,
    pub right: *mut Expression,
}

#[repr(C)]
pub struct UnaryExpression
{
    pub op: UnaryOp,
    pub left: *mut Expression,
}


#[repr(C)]
pub struct Variable
{
    pub visibility: TypeVisibility,
    pub complete_type: CompleteType,
    pub name: *const c_char,
    pub value: *mut Expression, //null if value undefined
}

#[repr(C)]
pub struct VariableStatement
{
    pub num_variables: u8,
    pub variables: *mut Variable,
}


#[repr(C)]
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
pub struct Statement
{
    pub statement_type: StatementType,
    pub statement_value: StatementValue,
}



#[repr(C)]
pub struct BlockStatement
{
    pub num_statements: u8,
    pub statements: *mut Statement,
}

#[repr(C)]
pub struct IfStatement
{
    pub condition: *mut Expression,
    pub statement: *mut Statement,
}

#[repr(C)]
pub struct WhileStatement
{
    pub condition: *mut Expression,
    pub statement: *mut Statement,
}

#[repr(C)]
pub struct ReturnStatement
{
    pub value: *mut Expression,
}

#[repr(C)]
pub struct NilStatement
{
    //null
}


#[repr(C)]
pub struct Argument
{
    pub name: *const c_char,
    pub complete_type: CompleteType,
}

#[repr(C)]
pub struct Arguments
{
    pub num_arguments: u8,
    pub arguments: *mut Argument,
}


#[repr(C)]
pub struct Function
{
    pub name: *const c_char,
    pub return_type: CompleteType, //void must be 'val' qualified
    pub arguments: Arguments,
    pub body: *mut Statement, //null if function body undefined
}

#[repr(C)]
pub struct Program
{
    pub num_functions: u8,
    pub functions: *mut Function,
}





fn convert_string(old: *const c_char) -> String
{
    let cstr = unsafe { CStr::from_ptr(old) };
    return String::from_utf8_lossy(cstr.to_bytes()).to_string();
}

fn add_to_symbol_table(identifier: *const c_char)
{
    //growable vector logic
    //check not in table already
    //throw error if exists
}





fn generate_for_expression(expression: *const Expression)
{

}

fn generate_for_statement(statement: *const Statement)
{
    match (*statement).statement_type
    {
        StatementType::BLOCK    => generate_block(),
        StatementType::IF       => generate_if(),
        StatementType::WHILE    => generate_while(),
        StatementType::RETURN   => generate_return(),
        StatementType::VARIABLE => generate_variable(),
        StatementType::NIL      => println!("Null statement ';' encountered. Was this intentional?"),
        _                       => println!("Invalid statement type");
    }
}

fn generate_for_function(function: *const Function)
{
    add_to_symbol_table((*function).name);

    generate_for_statement((*function).statement);
}




//will be called by the C frontend
#[no_mangle]
pub extern "C" fn generate(program: *const Program)
{
    for i in 0..(*program).num_functions
    {
        generate_for_function((*program).functions[i]);
    }
}
