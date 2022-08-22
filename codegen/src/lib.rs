mod bindings;
mod driver;
mod error;
mod expression;
mod statement;
mod util;

use bindings::*;
use error::*;
use driver::*;
use statement::*;
use util::*;

use std::{fs::*, io::Write};

fn generate_function(driver: &mut Driver, function: *const Function)
{
    unsafe
    {
        let fname = convert_string((*function).name);
        let fdefined = !(*function).body.is_null();


        driver.add_to_symbol_table(Symbol{ name: fname.clone(), defined: fdefined, symbol_type: SymbolType::FUNCTION });

        driver.add_to_code(format!("@function_start_{}:", &fname));

        if fdefined
        {
            generate_statement(driver, (*function).body, function);
        }

        driver.add_to_code(format!("@function_end_{}:", &fname));
        driver.add_to_code("pop IP".to_string());
    }
}


//will be called by the C frontend
//basically main()
#[no_mangle]
pub extern "C" fn generate(program: *const Program, parameters: *const Parameters)
{
    let mut driver = Driver::reset_driver();

    driver.add_to_code(".begin\n".to_string());
    driver.add_to_code(".include \"def.s\"\n".to_string());

    let mut found_main = false;

    unsafe
    {
        for i in 0..(*program).num_functions
        {
            let fun = (*program).functions.offset(i.into());

            generate_function(&mut driver, fun);

            if convert_string((*fun).name) == "main"
            {
                found_main = true;
            }
        }
    }

    if !found_main
    {
        codegen_error("No function 'main' was defined.".to_owned());
    }

    driver.add_to_code(".end".to_string());

    let mut file = File::create("test.ti.o").expect("unable to create file"); //quick and easy, but error prone file opening
    file.write_all(driver.code_segment.as_bytes()).expect("unable to write data");
}

