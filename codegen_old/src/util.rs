use std::ffi::CStr;
use std::os::raw::c_char;

pub fn convert_string(old: *const c_char) -> String
{
	let cstr = unsafe { CStr::from_ptr(old) };
	return String::from_utf8_lossy(cstr.to_bytes()).to_string();
}