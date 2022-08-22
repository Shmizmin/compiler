use super::bindings::*;
use super::error::*;


pub enum SymbolType
{
    VARIABLE
    {
        allocated_location: Location,
    },
    FUNCTION,
}


pub struct Symbol
{
    pub name: String,
    pub defined: bool,
    pub symbol_type: SymbolType,
}



pub struct Driver
{
    pub symbol_table: Vec<Symbol>,
    pub code_segment: String,
    pub available_registers: u8,
    pub counter: u32,
    pub available_heap: [bool; 0x4000],
    pub stack_pointer: u16,
}

impl Driver
{
    pub fn add_to_symbol_table(&mut self, symbol: Symbol)
    {
        //check that symbol name is undefined
        for i in self.symbol_table
        {
            if i.name == symbol.name
            {
                codegen_error(format!("Duplicate identifier {} was used.", symbol.name));
            }
        }

        self.symbol_table.push(symbol);
    }

    pub fn add_to_code(&mut self, value: String)
    {
        self.code_segment.push_str(value.as_str());
    }


    pub fn allocate(&mut self) -> Location
    {
        //short circuit stack allocations--much more frequent
        if (self.available_registers & (R0 | R1 | R2 | R3)) == 0
        {
            return STACK;
        }

        else //at least one register available
        {
            if (self.available_registers & R0) != 0
            {
                self.available_registers &= !R0;
                return R0;
            }
            else if (self.available_registers & R1) != 0
            {
                self.available_registers &= !R1;
                return R1;
            }
            else if (self.available_registers & R2) != 0
            {
                self.available_registers &= !R2;
                return R2;
            }
            else //register 3 is available by default
            {
                self.available_registers &= !R3;
                return R3;
            }
        }
    }

    pub fn deallocate(&mut self, location: Location)
    {
        match location
        {
            R0 => self.available_registers |= R0,
            R1 => self.available_registers |= R1,
            R2 => self.available_registers |= R2,
            R3 => self.available_registers |= R3,
        }
    }


    fn modify(&mut self, instruction: String, size: TypeSpecifier, df1: String, df2: String) -> u16
    {
        self.add_to_code(instruction);

        let diff;
        //support 'val' only types for now
        match size
        {
            BYTE => diff = 1,
            WORD => diff = 2,
            VOID => codegen_error(format!("Cannot {} type 'void' {} the stack", df1, df2)),
        }
        return diff;
    }

    pub fn push(&mut self, instruction: String, size: TypeSpecifier)
    {
        let diff = self.modify(instruction, size, "push".to_string(), "to".to_string());
        let staged = self.stack_pointer - diff;

        if staged < 0x3FFF
        {
            codegen_error(format!(
                "Stack was requested to grow to {} bytes from {} bytes, but cannot grow beyond 16,384 bytes", 
                    0x7FFF - staged, 0x7FFF - self.stack_pointer))
        }
        
        self.stack_pointer = staged;
        
    }

    pub fn pop(&mut self, instruction: String, size: TypeSpecifier)
    {
        let diff = self.modify(instruction, size, "pop".to_string(), "off of".to_string());
        let staged = self.stack_pointer + diff;
        
        if staged > 0x3FFF
        {
            codegen_error(format!(
                "Stack was requested to shrink to {} bytes from {} bytes, but cannot shrink beyond 0 bytes", 
                    0x7FFF - staged, 0x7FFF - self.stack_pointer));
        }

        self.stack_pointer = staged;
    }


    pub fn getsize(&mut self, complete_type: CompleteType) -> i32
    {
        if matches!(complete_type.type_qualifier, TypeQualifier::PTR)
        {
            return 2;
        }
        else 
        {
            use TypeSpecifier::*;
            match complete_type.type_specifier
            {
                BYTE => return 1,
                WORD => return 2,
                VOID => codegen_error("Type 'void' is unsized".to_string()),
            }
        }
    }

    pub fn allocate_heap(&mut self, complete_type: CompleteType)
    {
        let size = self.getsize(complete_type);
        let len = self.available_heap.len();
        for i in 0..len
        {
            match size
            {
                1 => if self.available_heap[i] { return i; },
                2 => if i < len && self.available_heap[i] && self.available_heap[i + 1] { return i },
                _ => codegen_error(format!("Heap allocations of size {} are not supported", size)),
            }
        }
    }

    pub fn deallocate_heap(&mut self, location: u16)
    {

    }

}

pub fn location_to_string(location: Location) -> String
{
    match location
    {
        R0    => return "r0".to_string(),
        R1    => return "r1".to_string(),
        R2    => return "r2".to_string(),
        R3    => return "r3".to_string(),
        STACK => return "stack".to_string(),
    }
}

pub const R0 : u8 = 1;
pub const R1 : u8 = 2;
pub const R2 : u8 = 4;
pub const R3 : u8 = 8;

pub const HEAP: u8 = 64; //only used for variables
pub const STACK : u8 = 128;

pub type Location = u8;

