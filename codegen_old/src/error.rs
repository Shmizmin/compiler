pub fn codegen_error(msg: String) -> !
{
    println!("[Error] {}\nExiting...\n", msg);
    panic!();
}

pub fn codegen_warning(msg: String)
{
    println!("[Warning] {}.\n", msg);
}