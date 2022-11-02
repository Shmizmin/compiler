#include "compiler.hpp"
#include "allocation.hpp"
#include "error.hpp"
#include "types.hpp"

#include <fmt/format.h>

namespace ti
{
    int generate_uuid(void) noexcept
    {
        static int uuid = 0;
        return uuid++;
    }
    
    
    void Compiler::add_to_symbol_table(Symbol* symbol) noexcept
    {
        bool found = false;
        for (const auto s : symbol_table)
        {
            //when comparing, we only look at name
            //this means that a function and variable cannot share the same identifier token
            if (s->name == s->name)
            {
                found = true;
            }
        }
        
        if (!found)
        {
            symbol_table.emplace_back(symbol);
        }
    }
    
    void Compiler::emit_label(const std::string& name) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::LABEL,
            .as.label.name = std::move(name),
        });
    }
    
    void Compiler::emit_ascii(const std::string& name) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::DIRECTIVE,
            .as.directive.type = DirectiveType::ASCII,
            .as.directive.as.ascii.text = std::move(name),
        });
    }
    
    void Compiler::emit_org(std::uint16_t address) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::DIRECTIVE,
            .as.directive.type = DirectiveType::ORIGIN,
            .as.directive.as.origin.address = address,
        });
    }
    
    void Compiler::emit_org(const HeapAllocation& allocation) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::DIRECTIVE,
            .as.directive.type = DirectiveType::ORIGIN,
            .as.directive.as.origin.address = allocation.address(),
        });
    }
    
    void Compiler::emit_jmp(ti::insn::Jmp::Condition condition, const std::string& label) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::INSTRUCTION,
            .as.instruction.type = InstructionType::JMP,
            .as.instruction.as.jmp.cond = condition,
            .as.instruction.as.jmp.label = label,
        });
    }
    
#define MATH_OPERATION_REG_IMM(x, y) \
    void Compiler::emit_##x(const RegisterAllocation& allocation, std::uint8_t value) noexcept \
    { \
        ir_code.emplace_back(new Command \
        { \
            .type = CommandType::INSTRUCTION, \
            .as.instruction.type = InstructionType::MATH, \
            .as.instruction.as.math.op = insn::Math::Operation::y, \
            .as.instruction.as.math.op1 = Operand \
            { \
                .type = OperandType::REG, \
                .as.reg.location = allocation.location, \
            }, \
            .as.instruction.as.math.op2 = Operand \
            { \
                .type = OperandType::IMM, \
                .as.imm.value = value, \
            }, \
        }); \
    }
    
#define MATH_OPERATION_REG_REG(x, y) \
    void Compiler::emit_##x(const RegisterAllocation& destination, const RegisterAllocation& source) noexcept \
    { \
        ir_code.emplace_back(new Command \
        { \
            .type = CommandType::INSTRUCTION, \
            .as.instruction.type = InstructionType::MATH, \
            .as.instruction.as.math.op = insn::Math::Operation::y, \
            .as.instruction.as.math.op1 = Operand \
            { \
                .type = OperandType::REG, \
                .as.reg.location = destination.location, \
            }, \
            .as.instruction.as.math.op2 = Operand \
            { \
                .type = OperandType::REG, \
                .as.reg.location = source.location, \
            }, \
        }); \
    }
    
    MATH_OPERATION_REG_IMM(adc, ADC)
    MATH_OPERATION_REG_REG(adc, ADC)
    
    MATH_OPERATION_REG_IMM(sbb, SBB)
    MATH_OPERATION_REG_REG(sbb, SBB)
    
    MATH_OPERATION_REG_IMM(and, LAND)
    MATH_OPERATION_REG_REG(and, LAND)
    
    MATH_OPERATION_REG_IMM(lor, LOR)
    MATH_OPERATION_REG_REG(lor, LOR)
    
    MATH_OPERATION_REG_IMM(rol, ROL)
    MATH_OPERATION_REG_IMM(ror, ROR)
    
#undef MATH_OPERATION_REG_IMM
#undef MATH_OPERATION_REG_REG
  
    
    void Compiler::emit_not(const RegisterAllocation& allocation) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::INSTRUCTION,
            .as.instruction.type = InstructionType::MATH,
            .as.instruction.as.math.op = insn::Math::Operation::LNOT,
            .as.instruction.as.math.op1 = Operand
            {
                .type = OperandType::REG,
                .as.reg.location = allocation.location,
            },
        });
    }
    
    void Compiler::emit_stb(std::uint16_t address, const RegisterAllocation& allocation) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::INSTRUCTION,
            .as.instruction.type = InstructionType::MOVE,
            .as.instruction.as.move.op1 = Operand
            {
                .type = OperandType::MEM,
                .as.mem.address = address,
            },
            .as.instruction.as.move.op2 = Operand
            {
                .type = OperandType::REG,
                .as.reg.location = allocation.location,
            },
        });
    }
    
    void Compiler::emit_stb(const HeapAllocation& memory, const RegisterAllocation& allocation) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::INSTRUCTION,
            .as.instruction.type = InstructionType::MOVE,
            .as.instruction.as.move.op1 = Operand
            {
                .type = OperandType::MEM,
                .as.mem.address = memory.address(),
            },
            .as.instruction.as.move.op2 = Operand
            {
                .type = OperandType::REG,
                .as.reg.location = allocation.location,
            },
        });
    }
    
    void Compiler::emit_mvb(const RegisterAllocation& destination, const RegisterAllocation& source) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::INSTRUCTION,
            .as.instruction.type = InstructionType::MOVE,
            .as.instruction.as.move.op1 = Operand
            {
                .type = OperandType::REG,
                .as.reg.location = destination.location,
            },
            .as.instruction.as.move.op2 = Operand
            {
                .type = OperandType::REG,
                .as.reg.location = source.location,
            },
        });
    }
    
    void Compiler::emit_ldb(const RegisterAllocation& allocation, std::uint8_t value) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::INSTRUCTION,
            .as.instruction.type = InstructionType::MOVE,
            .as.instruction.as.move.op1 = Operand
            {
                .type = OperandType::REG,
                .as.reg.location = allocation.location,
            },
            .as.instruction.as.move.op2 = Operand
            {
                .type = OperandType::IMM,
                .as.imm.value = value,
            },
        });
    }
    
    void Compiler::emit_call(const RegisterAllocation& allocation, const std::string& label) noexcept
    {
        emit_and(allocation, 0);
        emit_jmp(ti::insn::Jmp::Condition::JEZ, label);
        emit_pop(allocation);
    }
    
#define STACK_OPERATION_REG(x, y) \
    void Compiler::emit_##x(const RegisterAllocation& allocation) noexcept \
    { \
        ir_code.emplace_back(new Command \
        { \
            .type = CommandType::INSTRUCTION, \
            .as.instruction.type = InstructionType::STACK, \
            .as.instruction.as.stack.op = Operand \
            { \
                .type = OperandType::REG, \
                .as.reg.location = allocation.location, \
            }, \
            .as.instruction.as.stack.dir = insn::Stack::Direction::y, \
        }); \
    }
    
    STACK_OPERATION_REG(push, PUSH)
    STACK_OPERATION_REG(pop, POP)
    
#undef STACK_OPERATION_REG
}

namespace ti
{
    std::uint8_t get_size_by_type(CompleteType type) noexcept
    {
        using enum TypeSpecifier;
        switch (type.specifier)
        {
            case BYTE: return 1; break;
            case VOID: throw_error("Type \'void\' is unsized");          break;
            default:   throw_error("Unknown type to query the size of"); break;
        }
    }
}
