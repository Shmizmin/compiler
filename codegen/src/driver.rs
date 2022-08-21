use super::error::*;

pub enum SymbolType
{
    FUNCTION,
    VARIABLE,
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
            R0 => self.available_registers &= !R0,
            R1 => self.available_registers &= !R1,
            R2 => self.available_registers &= !R2,
            R3 => self.available_registers &= !R3,
        }
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
pub const STACK : u8 = 128;

pub type Location = u8;

