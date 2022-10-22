#include "Context.hpp"
#include "Error.hpp"
#include "Central.hpp"
#include "Types.hpp"
#include "fmt/format.h"

#include <algorithm>


namespace ti
{
    int generate_uuid(void) noexcept
    {
        static int uuid = 0;
        return uuid++;
    }
    
    
    void Context::add_to_symbol_table(Symbol* symbol) noexcept
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
    
    std::optional<RegisterType> Context::allocate_register(void) noexcept
    {
        using enum RegisterType;
        if (available_registers[0])
        {
            available_registers[0] = false;
            return { R0 };
        }
        else if (available_registers[1])
        {
            available_registers[1] = false;
            return { R1 };
        }
        else if (available_registers[2])
        {
            available_registers[2] = false;
            return { R2 };
        }
        else if (available_registers[3])
        {
            available_registers[3] = false;
            return { R3 };
        }
        else
        {
            return std::nullopt;
        }
    }
    
    RegisterType Context::allocate_register_forced(void) noexcept
    {
        auto new_allocation = allocate_register();
        
        if (!new_allocation.has_value())
        {
            emit_push(RegisterType::R0);
            new_allocation.value() = RegisterType::R0;
            available_registers[0] = false;
        }
        
        return new_allocation.value();
    }
    
    RegisterType Context::allocate_register_forced_exclude(RegisterType exclude) noexcept
    {
        auto new_allocation = allocate_register();
        
        if (!new_allocation.has_value())
        {
            auto& value = new_allocation.value();
            
            using enum RegisterType;
            switch (value)
            {
                case R0: value = R1; available_registers[1] = false; break;
                case R1: value = R2; available_registers[2] = false; break;
                case R2: value = R3; available_registers[3] = false; break;
                case R3: value = R0; available_registers[0] = false; break;
                default:                                             break;
            }
            
            emit_push(value);
        }
        
        return new_allocation.value();
    }
    
    void Context::deallocate_register(RegisterType location) noexcept
    {
        using enum RegisterType;
        switch (location)
        {
            case R0: available_registers[0] = true; break;
            case R1: available_registers[1] = true; break;
            case R2: available_registers[2] = true; break;
            case R3: available_registers[3] = true; break;
            default:                                break;
        }
    }
    
    void Context::deallocate_register_forced(RegisterType location) noexcept
    {
        emit_pop(location);
        deallocate_register(location);
    }
    
    std::uint16_t Context::allocate_heap(std::uint16_t bytes) noexcept
    {
        auto available = true;
        for (auto i = 0u; i < available_heap.size(); ++i)
        {
            for (auto j = 0u; j < bytes; ++j)
            {
                if (!available_heap[i + j])
                    available = false;
            }
            
            if (available)
            {
                for (auto k = 0u; k < bytes; ++k)
                    available_heap[i + k] = false;
                
                return static_cast<std::uint16_t>(i);
            }
            
            else
                available = true;
            
        }
        
        if (!available)
        {
            ti::throw_error(fmt::format("No contiguous heap region of {} bytes available", bytes));
        }
    }
    
    void Context::deallocate_heap(std::uint16_t address, std::uint16_t bytes) noexcept
    {
        for (auto i = address; i < address + bytes; ++i)
            available_heap[i] = true;
    }
    
    
    void Context::emit_label(const std::string& name) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::LABEL,
            .as.label.name = std::move(name),
        });
    }
    
    void Context::emit_ascii(const std::string& name) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::DIRECTIVE,
            .as.directive.type = DirectiveType::ASCII,
            .as.directive.as.ascii.text = std::move(name),
        });
    }
    
    void Context::emit_jmp(ti::insn::Jmp::Condition condition, const std::string& label) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::INSTRUCTION,
            .as.instruction.type = InstructionType::JMP,
            .as.instruction.as.jmp.cond = condition,
            .as.isntruction.as.jmp.label = label,
        });
    }
    
    void Context::emit_adc(RegisterType location, std::uint8_t value) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::INSTRUCTION,
            .as.instruction.type = InstructionType::MATH,
            .as.instruction.as.math.op = insn::Math::Operation::ADC,
            .as.instruction.as.math.op1 = Operand
            {
                .type = OperandType::REG,
                .as.reg.location = location,
            },
            .as.instruction.as.math.op2 = Operand
            {
                .type = OperandType::IMM,
                .as.imm.value = value,
            },
        });
    }
    
    void Context::emit_and(RegisterType location, std::uint8_t value) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::INSTRUCTION,
            .as.instruction.type = InstructionType::MATH,
            .as.instruction.as.math.op = insn::Math::Operation::LAND,
            .as.instruction.as.math.op1 = Operand
            {
                .type = OperandType::REG,
                .as.reg.location = location,
            },
            .as.instruction.as.math.op2 = Operand
            {
                .type = OperandType::IMM,
                .as.imm.value = value,
            },
        });
    }
    
    void Context::emit_lor(RegisterType location, std::uint8_t value) noexcept
    {
        ir_code.emplace_back(new Command
        {
            .type = CommandType::INSTRUCTION,
            .as.instruction.type = InstructionType::MATH,
            .as.instruction.as.math.op = insn::Math::Operation::LOR,
            .as.instruction.as.math.op1 = Operand
            {
                .type = OperandType::REG,
                .as.reg.location = location,
            },
            .as.instruction.as.math.op2 = Operand
            {
                .type = OperandType::IMM,
                .as.imm.value = value,
            },
        });
    }
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
    
    std::string regtype_to_string(RegisterType regtype) noexcept
    {
        using enum RegisterType;
        switch (regtype)
        {
            case R0: return "r0";    break;
            case R1: return "r1";    break;
            case R2: return "r2";    break;
            case R3: return "r3";    break;
            case RF: return "flags"; break;
            case IP: return "ip";    break;
        }
    }
}
