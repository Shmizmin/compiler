use super::bindings::*;
use super::error::*;


pub enum SymbolType
{
    VARIABLE
    {
        allocated_location: Location,
        address: u16,
        visibility: TypeVisibility,
        parent_function: *const Function,
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
    //essentially a constructor for this struct
    pub fn reset_driver() -> Driver
    {

        return Driver
        {
            symbol_table: vec![],
            code_segment: "".to_string(),
            available_registers: (R0 | R1 | R2 | R3),
            counter: 0,
            available_heap: [true; 0x4000],
            stack_pointer: 0x7FFF,
        };
    }


    //verifies that a symbol of the same name doesn't exist
    //this means that functions and variables cannot share the same name
    pub fn add_to_symbol_table(&mut self, symbol: Symbol)
    {

        //check that symbol name is undefined
        for i in &self.symbol_table
        {
            if i.name == symbol.name
            {
                codegen_error(format!("Duplicate identifier {} was used", symbol.name));
            }
        }

        self.symbol_table.push(symbol);
    }


    //implement better error handling for potentially invalid characters in the string 
    //(may come from C and potentially be corrupted)
    pub fn add_to_code(&mut self, value: String)
    {
        self.code_segment.push_str(value.as_str());
    }


    //will always return a register allocation
    //saves an old allocation if needbe
    pub fn force_allocate(&mut self) -> (u8, bool)
    {

        let mut alloc_name = self.allocate();

        //if no register available, make room
        let mut was_forced = false;
        if alloc_name == STACK
        {
            self.add_to_code("push r0\n".to_string());
            alloc_name = R0;
            was_forced = true;
        }

        return (alloc_name, was_forced);
    }


    //name is a misnomer--is just the complement of 'force_allocate'
    //determine if needed to clean up a complete expression generation
    pub fn force_deallocate(&mut self, allocation: (u8, bool))
    {
        //was forced
        if allocation.1
        {
            self.add_to_code("pop r0\n".to_string());
        }
        else
        {
            self.deallocate(allocation.0);
        }
    }



    //returns a valid register or tells to allocate on the stack
    //not often used externally
    //many operations require registers in order to directly access the ALU
    //improve the allocation algorithm so high importance variables are always in registers
    pub fn allocate(&mut self) -> Location
    {
        //short circuit stack allocations--usually more frequent
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


    //reverse of above
    pub fn deallocate(&mut self, location: Location)
    {
        match location
        {
            R0 => self.available_registers |= R0,
            R1 => self.available_registers |= R1,
            R2 => self.available_registers |= R2,
            R3 => self.available_registers |= R3,
            _      => codegen_error("Unrecognized location to deallocate".to_string()),
        }
    }


    //internal helper function
    fn modify(&mut self, instruction: String, size: TypeSpecifier, df1: String, df2: String) -> u16
    {

        self.add_to_code(instruction);

        let diff;
        //support 'val' only types for now
        use TypeSpecifier::*;
        match size
        {
            BYTE => diff = 1,
            WORD => diff = 2,
            VOID => codegen_error(format!("Cannot {} type 'void' {} the stack", df1, df2)),
        }
        return diff;
    }


    //allocate space on the stack for a value of a designed size to pushed onto
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


    //deallocate space on the stack for a value of a designed size to be popped off from
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


    //simply returns the size in bytes of the various types that Titanium supports
    //pointers aren't yet implemented despite being correctly sized
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


    //smart allocate heap memory virtually
    //searches heap to find earliest spot to slot new variables into
    //because of the small allocation sizes, it's unlikely that
    //A: the heap runs out of capacity (unlike the stack due to recursive functions)
    //B: the fragmentation beceomes a significant problem
    //currently not easily accessible by the programmer in Titanium
    //many variables will automatically be put on the heap for register optimization reasons
    //addrof/deref will basically just return address/object of a variable
    pub fn allocate_heap(&mut self, complete_type: CompleteType) -> u16
    {

        let size = self.getsize(complete_type);
        let len = self.available_heap.len();
        for i in 0..len
        {
            let cvt: u16 = i.try_into().unwrap();

            match size
            {
                1 => if self.available_heap[i] 
                { 
                    self.available_heap[i] = false;
                    return cvt;
                },
                2 => if i < len && self.available_heap[i] && self.available_heap[i + 1] 
                { 
                    self.available_heap[i] = false;
                    self.available_heap[i + 1] = false;
                    return cvt; 
                },
                _ => codegen_error(format!("Heap allocations of size {} are unsupported", size)),
            }
        }
        //went through heap--no slots or none large enough
        codegen_error(format!("No contiguous region of heap large enough to allocate {} bytes", size));
    }



    pub fn deallocate_heap(&mut self, location: u16, complete_type: CompleteType)
    {
        let len = self.available_heap.len();

        if location < 0x4000 && location < len.try_into().unwrap()
        {
            let locc: usize = location.into();
            let size = self.getsize(complete_type);

            match size
            {
                1 => self.available_heap[locc] = true,
                2 =>
                {
                    self.available_heap[locc] = true;
                    self.available_heap[locc + 1] = true;
                }
                _ => codegen_error(format!("Heap deallocations of size {} are unsupported", size)), 
            }
        }
        else
        {
            codegen_error(format!("Address {} is outside the heap region and cannot be deallocated", location));
        }
    }

}


//useful for generic functions when register allocations aren't guaranteed to be in the same place each time
pub fn location_to_string(location: Location) -> String
{
    match location
    {
        R0    => return "r0".to_string(),
        R1    => return "r1".to_string(),
        R2    => return "r2".to_string(),
        R3    => return "r3".to_string(),
        STACK => return "stack".to_string(),
        _         => codegen_error("Unknown location to convert to string".to_string()),
    }
}

//useful for error handling to generically print messages for each type of expression
//for instance: Expected a string constant, not an identifier
pub fn expression_type_to_string(expression_type: ExpressionType) -> String
{
    use ExpressionType::*;
    match expression_type
    {
        NUMCONST => return "numeric constant".to_string(),
        STRINGCONST => return "string constant".to_string(),
        IDENTIFIER => return "identifier".to_string(),
        TERNARYOP => return "ternary operator".to_string(),
        BINARYOP => return "binary operator".to_string(),
        UNARYOP => return "unary operator".to_string(),
    }
}

//should really be in an enum
//this Rust implementation is mangled by the required C interoperability
//an easy to use interface for a standard container to wrap the register allocation/deallocation
//process simply wasn't in the cards for this version
//ideally a vec4<bool> would represent the registers, but the kludginess to get 'r0-3' indexing working isn't
//really worth the small savings in readability for this project
pub const R0 : u8 = 1;
pub const R1 : u8 = 2;
pub const R2 : u8 = 4;
pub const R3 : u8 = 8;

pub const HEAP: u8 = 64; //only used for variables
pub const STACK : u8 = 128;

pub type Location = u8;

