use super::bindings::*;
use super::driver::*;


pub fn generate_numconst(driver: &mut Driver, numconst_expression: *const NumconstExpression, parent_function: *const Function, return_location: Location)
{

}

pub fn generate_stringconst(driver: &mut Driver, stringconst_expression: *const StringconstExpression, parent_function: *const Function, return_location: Location)
{

}

pub fn generate_identifier(driver: &mut Driver, identifier_expression: *const IdentifierExpression, parent_function: *const Function, return_location: Location)
{

}

pub fn generate_ternaryop(driver: &mut Driver, ternary_expression: *const TernaryExpression, parent_function: *const Function, return_location: Location)
{

}

pub fn generate_binaryop(driver: &mut Driver, binary_expression: *const BinaryExpression, parent_function: *const Function, return_location: Location)
{

}

pub fn generate_unaryop(driver: &mut Driver, unary_expression: *const UnaryExpression, parent_function: *const Function, return_location: Location)
{
    unsafe
    {
        let uop = (*unary_expression).op;
        let uleft = (*unary_expression).left;

        let unary_alloc = driver.allocate();
        let alloc_name = location_to_string(unary_alloc);

        match uop
        {
            PLUS_PLUS =>
            MINUS_MINUS =>

            ADDROF =>
            DEREF =>

            POSITIVE =>
            NEGATIVE =>
        }


        driver.deallocate(unary_alloc);
    }
}


pub fn generate_expression(driver: &mut Driver, expression: *const Expression, parent_function: *const Function, return_location: Location)
{
    unsafe
    {
        let etype = (*expression).expression_type;
        let evalue = (*expression).expression_value;

        let alloc_loc = driver.allocate();

        match etype
        {
            NUMCONST    => generate_numconst(driver, evalue.numconst_expression, parent_function, alloc_loc),
            STRINGCONST => generate_stringconst(driver, evalue.stringconst_expression, parent_function, alloc_loc),
            IDENTIFIER  => generate_identifier(driver, evalue.identifier_expression, parent_function, alloc_loc),
            TERNARYOP   => generate_ternaryop(driver, evalue.ternary_expression, parent_function, alloc_loc),
            BINARYOP    => generate_binaryop(driver, evalue.binary_expression, parent_function, alloc_loc),
            UNARYOP     => generate_unaryop(driver, evalue.unary_expression, parent_function, alloc_loc),
        }

        driver.deallocate(alloc_loc);
    }
}