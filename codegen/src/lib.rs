mod bindings;
mod error;
mod util;

use bindings::*;
use error::*;
use util::*;


struct Symbol
{
    name: String,
    defined: bool,
}

struct Driver
{
    symbol_table: Vec<Symbol>,
    code_segment: String,
}





fn add_to_symbol_table(driver: &mut Driver, name: String, defined: bool)
{
    driver.symbol_table.push(Symbol{ name, defined });
}

fn add_to_code(driver: &mut Driver, value: String)
{
    driver.code_segment.push_str(value.as_str());
}



fn generate_expression(expression: *const Expression)
{

}




fn generate_block(driver: &mut Driver, block_statement: *const BlockStatement, parent_function: *const Function)
{
    unsafe
    {
        for i in 0..(*block_statement).num_statements
        {
            generate_statement(driver, (*block_statement).statement, parent_function);
        }
    }
}

fn generate_if(driver: &mut Driver, if_statement: *const IfStatement, parent_function: *const Function)
{

}

fn generate_while(driver: &mut Driver, while_statement: *const WhileStatement, parent_function: *const Function)
{
    unsafe
    {
        let label = format!("@while_begin_{}_{}", (*parent_function).name, )

        add_to_code(driver, label);

        generate_statement(driver, (*while_statement).statement, parent_function);

        generate_expression(driver, (*while_statement).condition, parent_function);
    }
}

fn generate_return(driver: &mut Driver, return_statement: *const ReturnStatement, parent_function: *const Function)
{

}

fn generate_variable(driver: &mut Driver, variable_statement: *const VariableStatement, parent_function: *const Function)
{

}

fn generate_nil(driver: &mut Driver, nil_statement: *const NilStatement, parent_function: *const Function)
{
    codegen_warning("Null statement ';' encountered.");
}

fn generate_error(driver: &mut Driver, parent_function: *const Function)
{
    codegen_error("Invalid statement type encountered");
}



fn generate_statement(driver: &mut Driver, statement: *const Statement, parent_function: *const Function)
{
    unsafe
    {
        let stype = &((*statement).statement_type);
        let svalue = &((*statement).statement_value);

        match stype
        {
            BLOCK    => generate_block(driver, svalue.block_statement, parent_function),
            IF       => generate_if(driver, svalue.if_statement, parent_function),
            WHILE    => generate_while(driver, svalue.while_statement, parent_function),
            RETURN   => generate_return(driver, svalue.return_statement, parent_function),
            VARIABLE => generate_variable(driver, svalue.variable_statement, parent_function),
            NIL      => generate_nil(driver, svalue.nil_statement, parent_function),
            _                        => generate_error(driver, parent_function),
        }
    }
}

fn generate_function(driver: &mut Driver, function: *const Function)
{
    unsafe
    {
        let fname = convert_string((*function).name);
        let fdefined = !(*function).statement.is_null();

        add_to_symbol_table(driver, fname, fdefined);

        add_to_code(driver, format!("@function_start_{}:", name));

        if defined
        {
            generate_statement(driver, (*function).statement, function);
        }

        add_to_code(driver, "pop IP".to_string());
    }
}




//will be called by the C frontend
#[no_mangle]
pub extern "C" fn generate(program: *const Program)
{
    let mut driver = Driver
    {
        //symbol_table: {}.
    };

    let mut driver: Driver{ (), "" };

    add_to_code(driver, ".begin\n");
    add_to_code(driver, ".include \"def.s\"");


    let found_main = false;

    unsafe
    {
        for i in 0..(*program).num_functions
        {
            let fun = (*program).functions[i];
            generate_function(driver, fun);

            if (*fun).name == "main"
            {
                found_main = true;
            }
        }
    }

    if (!found_main)
    {
        codegen_error("No function 'main' was defined.");
    }

    add_to_code(driver, ".end");
}

