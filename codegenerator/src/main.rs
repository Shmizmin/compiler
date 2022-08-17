use std::ffi::Cstr;
use std::os::raw::c_char;


struct c_Function
{
    identifier: *const c_char;
}

pub fn convert_string(old: *const c_char)
{
    let cstr = unsafe { CStr::from_ptr(old) };
    return String::from_utf8_lossy(cstr.to_bytes()).to_string();
}



pub fn generate_internal(functions: &Vec<Function>)
{

}


//will be called by the C frontend
#[no_mangle]
pub extern "C" fn generate(num_functions: const u32, functions: *const c_Function)
{
    for (let i = 0; i < num_functions; i++)
    {
        
    }
}
