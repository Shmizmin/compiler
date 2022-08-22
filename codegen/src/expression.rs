use super::bindings::*;
use super::driver::*;
use super::error::*;

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
    unsafe
    {
        let bop = (*binary_expression).op;
        let bleft = (*binary_expression).left;
        let bright = (*binary_expression).right;

        let alloc_l = driver.force_allocate();
        let alloc_r = driver.allocate();
        
        generate_expression(driver, bleft, parent_function, alloc_l.0);
    }
}

pub fn generate_unaryop(driver: &mut Driver, unary_expression: *const UnaryExpression, parent_function: *const Function, return_location: Location)
{
    unsafe
    {
        let uop = (*unary_expression).op;
        let uleft = (*unary_expression).left;

        let alloc = driver.force_allocate();
        let reg = location_to_string(alloc.0);
        //generates code for the expression operand
        generate_expression(driver, uleft, parent_function, alloc.0);

        use UnaryOp::*;
        match uop
        {
            PLUS_PLUS => driver.add_to_code(format!("adc {}, #1\n", reg)),
            MINUS_MINUS => driver.add_to_code(format!("sbb {}, #1\n", reg)),

            ADDROF => {}, //pointers are currently unsupported
            DEREF => {}, //^

            POSITIVE => {()}, //is a no-op
            NEGATIVE => //two's complement
            {
                driver.add_to_code(format!("not {}\n", reg));
                driver.add_to_code(format!("adc {}, #1\n", reg));
            },
        }

        //guaranteed to be a move from reg to reg
        //parent caller 'force_allocate's
        if alloc.0 != return_location 
        {
            driver.add_to_code(format!("mvb {}, {}", location_to_string(return_location), reg));
        }
        
        //might be a bug--unclear register ownership semantics for freeing
        driver.force_deallocate(alloc);
    }
}


pub fn generate_expression(driver: &mut Driver, expression: *const Expression, parent_function: *const Function, return_location: Location)
{
    unsafe
    {
        let etype = &(*expression).expression_type;
        let evalue = &(*expression).expression_value;

        let alloc = STACK; //pretend that register allocated failed to save registers for actual expressions

        use ExpressionType::*;
        match etype
        {
            NUMCONST    => generate_numconst(driver, evalue.numconst_expression, parent_function, alloc),
            STRINGCONST => generate_stringconst(driver, evalue.stringconst_expression, parent_function, alloc),
            IDENTIFIER  => generate_identifier(driver, evalue.identifier_expression, parent_function, alloc),
            TERNARYOP   => generate_ternaryop(driver, evalue.ternary_expression, parent_function, alloc),
            BINARYOP    => generate_binaryop(driver, evalue.binary_expression, parent_function, alloc),
            UNARYOP     => generate_unaryop(driver, evalue.unary_expression, parent_function, alloc),
        }

        match return_location
        {
            R0 => driver.add_to_code("pop R0\n".to_string()),
            R1 => driver.add_to_code("pop R1\n".to_string()),
            R2 => driver.add_to_code("pop R2\n".to_string()),
            R3 => driver.add_to_code("pop R3\n".to_string()),
            STACK => (), //value is already on stack
            _ => codegen_error("Invalid expression return location".to_string()),
        }
    }
}