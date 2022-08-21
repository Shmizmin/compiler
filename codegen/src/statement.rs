use super::bindings::*;
use super::driver::*;
use super::expression::*;
use super::util::*;
use super::error::*;

pub fn generate_block(driver: &mut Driver, block_statement: *const BlockStatement, parent_function: *const Function)
{
    unsafe
    {
        for i in 0..(*block_statement).num_statements
        {
            generate_statement(driver, (*block_statement).statements, parent_function);
        }
    }
}

pub fn generate_if(driver: &mut Driver, if_statement: *const IfStatement, parent_function: *const Function)
{
    unsafe
    {
        let label = format!("@if_end_{}_{}", convert_string((*parent_function).name), driver.counter);
        let mut alloc_name = driver.allocate();

        if alloc_name == STACK
        {
            driver.add_to_code("push r0\n".to_string());
            alloc_name = R0;
        }

        generate_expression(driver, (*if_statement).condition, parent_function, alloc_name);
        driver.add_to_code("updateFlags()\n".to_string());
        driver.add_to_code(format!("jez [{}]", label));

        if alloc_name == STACK
        {
            driver.add_to_code("pop r0\n".to_string());
        }
        else
        {
            driver.deallocate(alloc_name);
        }

        generate_statement(driver, (*if_statement).statement, parent_function);

        driver.add_to_code(label);
    }
}

pub fn generate_while(driver: &mut Driver, while_statement: *const WhileStatement, parent_function: *const Function)
{
    unsafe
    {
        let label = format!("@while_begin_{}_{}", convert_string((*parent_function).name), driver.counter);
        let alloc_name = driver.allocate();

        if alloc_name == STACK
        {
            driver.add_to_code("push r0\n".to_string());
            alloc_name = R0;
        }

        driver.add_to_code(label);

        generate_statement(driver, (*while_statement).statement, parent_function);
        generate_expression(driver, (*while_statement).condition, parent_function, alloc_name);

        if alloc_name == STACK
        {
            driver.add_to_code("pop r0\n".to_string());
        }
        else
        {
            driver.deallocate(alloc_name);
        }

        driver.add_to_code("updateFlags()".to_string());
        driver.add_to_code(format!("jnz([{}])", label));
    }
}

pub fn generate_return(driver: &mut Driver, return_statement: *const ReturnStatement, parent_function: *const Function)
{
    unsafe
    {
        generate_expression(driver, (*return_statement).value, parent_function, STACK);
        driver.add_to_code(format!("jmp([{}])", convert_string((*parent_function).name)));
    }
}

pub fn generate_variable(driver: &mut Driver, variable_statement: *const VariableStatement, parent_function: *const Function)
{
    unsafe
    {
        
    }
}

pub fn generate_nil(driver: &mut Driver, nil_statement: *const NilStatement, parent_function: *const Function)
{
    codegen_warning("Null statement ';' encountered.".to_string());
}



pub fn generate_statement(driver: &mut Driver, statement: *const Statement, parent_function: *const Function)
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
            _                        => codegen_error("Invalid statement type encountered.".to_string()),
        }
    }
}