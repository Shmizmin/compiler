pub fn codegen_error(msg: String)
{
    println!("[Error] {}\nExiting...\n", msg);
    std::process::exit(1);
}

pub fn codegen_warning(msg: String)
{
    println!("[Warning] {}.\n", msg);
}