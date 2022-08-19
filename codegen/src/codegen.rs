#![allow(non_camel_case_types)]

use bindings;

use std::ffi::CStr;
use std::os::raw::c_char;


fn add_to_symbol_table(identifier: *const c_char)
{
    struct Symbol
    {
        name: String,
        defined: bool,
    }
    //very unsafe--internal use only
    static mut symbol_table: Vec<Symbol>;

    //growable vector logic
    //check not in table already
    //throw error if exists
    //functions and variables cannot share names
}




fn convert_string(old: *const c_char) -> String
{
    let cstr = unsafe { CStr::from_ptr(old) };
    return String::from_utf8_lossy(cstr.to_bytes()).to_string();
}







fn generate_expression(expression: *const Expression)
{

}




fn generate_block(block_statement: *const BlockStatement)
{
    println!("Block statement '{}' encountered");

    for i in 0..(*block_statement).num_statements
    {
        generate_statement((*block_statement).statement);
    }
}

fn generate_if(if_statement: *const IfStatement)
{

}

fn generate_while(while_statement: *const WhileStatement)
{

}

fn generate_return(return_statement: *const ReturnStatement)
{

}

fn generate_variable(variable_statement: *const VariableStatement)
{

}

fn generate_nil(nil_statement: *const NilStatement)
{
    println!("Null statement ';' encountered.");
}

fn generate_error()
{
    println!("Invalid statement type encountered.");
}



fn generate_statement(statement: *const Statement)
{
    let stype = &((*statement).statement_type);
    let svalue = &((*statement).statement_value);

    match stype
    {
        StatementType::BLOCK    => generate_block(svalue.block_statement),
        StatementType::IF       => generate_if(svalue.if_statement),
        StatementType::WHILE    => generate_while(svalue.while_statement),
        StatementType::RETURN   => generate_return(svalue.return_statement),
        StatementType::VARIABLE => generate_variable(svalue.variable_statement),
        StatementType::NIL      => generate_nil(svalue.nil_statement),
        _                       => generate_error();
    }
}

fn generate_function(function: *const Function)
{
    let fname = convert_string((*function).name);
    let fdefined = !(*function).statement.is_null();

    add_to_symbol_table(
        Symbol
        { 
            name: fname,
            defined: fdefined,
        });


    add_to_code(format!("@function_start_{}:", name));

    if defined
    {
        generate_statement((*function).statement);
    }
}




//will be called by the C frontend
#[no_mangle]
pub extern "C" fn generate(program: *const Program)
{
    for i in 0..(*program).num_functions
    {
        generate_function((*program).functions[i]);
    }
}
