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
        let alloc = driver.force_allocate();

        generate_expression(driver, (*if_statement).condition, parent_function, alloc.0);
        driver.add_to_code("updateFlags()\n".to_string());
        driver.add_to_code(format!("jez [{}]", label));

        driver.force_deallocate(alloc);


        generate_statement(driver, (*if_statement).statement, parent_function);

        driver.add_to_code(label);
    }
}

pub fn generate_while(driver: &mut Driver, while_statement: *const WhileStatement, parent_function: *const Function)
{
    unsafe
    {
        let label = format!("@while_begin_{}_{}", convert_string((*parent_function).name), driver.counter);
        let alloc = driver.force_allocate();

        driver.add_to_code(label);

        generate_statement(driver, (*while_statement).statement, parent_function);
        generate_expression(driver, (*while_statement).condition, parent_function, alloc.0);

        driver.force_deallocate(alloc);

        driver.add_to_code("updateFlags()".to_string());
        driver.add_to_code(format!("jnz([{}])", label));
    }
}

pub fn generate_return(driver: &mut Driver, return_statement: *const ReturnStatement, parent_function: *const Function)
{
    unsafe
    {
        //compute return value
        generate_expression(driver, (*return_statement).value, parent_function, STACK);
        //jump back to caller
        driver.add_to_code(format!("jmp([{}])", convert_string((*parent_function).name)));
    }
}

pub fn generate_variable(driver: &mut Driver, variable_statement: *const VariableStatement, parent_function: *const Function)
{
    unsafe
    {
        for i in 0..(*variable_statement).num_variables
        {
            let variable = *((*variable_statement).variables.offset(i.into()));
            let defined = !variable.value.is_null();
            let name = convert_string(variable.name);

            if matches!(variable.complete_type.type_specifier, TypeSpecifier::VOID)
            {
                codegen_error(format!("Variable {} illegally declared as type 'void'", name));
            }

            let address = driver.allocate_heap(variable.complete_type);
            let symbol = Symbol
            { 
                name, 
                defined, 
                symbol_type: SymbolType::VARIABLE
                { 
                    allocated_location: HEAP, 
                    address,
                    visibility: variable.visibility,
                    parent_function,
                }, 
            };

            driver.add_to_symbol_table(symbol);

            if defined
            {
                //this only support 1 byte values for now
                //words will be supported later for variable & expression mathematics
                let alloc = driver.force_allocate();

                generate_expression(driver, variable.value, parent_function, alloc.0);

                driver.add_to_code(format!("stb  %{}, {}", address, location_to_string(alloc.0)));

                driver.force_deallocate(alloc);
            }
        }
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