#include "bytecode.hpp"
#include "central.hpp"
#include "error.hpp"

#include <fmt/format.h>

namespace ti
{
    std::string generate_commands(const std::list<Command*>& commands) noexcept
    {
        std::string code;
        
        for (auto command_ptr : commands)
        {
            const auto& command = *command_ptr;
            
            using enum CommandType;
            switch (command.type)
            {
                case DIRECTIVE:
                {
                    using enum DirectiveType;
                    switch (command.as.directive.type)
                    {
                        case ORIGIN: code.append(fmt::format(".org {}",       command.as.directive.as.origin.address));     break;
                        case   BYTE: code.append(fmt::format(".byte {}",      command.as.directive.as.word.value));         break;
                        case   WORD: code.append(fmt::format(".word {}",      command.as.directive.as.word.value));         break;
                        case  ASCII: code.append(fmt::format(".ascii \"{}\"", command.as.directive.as.ascii.text.c_str())); break;
                    }
                } break;
                    
                case INSTRUCTION:
                {
                    constexpr auto cvt = [&](Operand op) noexcept -> std::string
                    {
                        using enum OperandType;
                        switch (op.type)
                        {
                            case REG: return regtype_to_string(op.as.reg.location);
                            case  IP: return "ip";
                            case MEM: return ("%" + std::to_string(op.as.mem.address));
                            case IMM: return ("#" + std::to_string(op.as.imm.value));
                        }
                    };
                    
                    
                    using enum InstructionType;
                    switch (command.as.instruction.type)
                    {
                        case NOP: code.append("\tnop\n"); break;
                        case BRK: code.append("\tbrk\n"); break;
                            
                        case MATH:
                        {
                            using enum insn::Math::Operation;
                            switch (command.as.instruction.as.math.op)
                            {
                                case  ADC: code.append("\tadc "); break;
                                case  SBB: code.append("\tsbb "); break;
                                case LAND: code.append("\tand "); break;
                                case  LOR: code.append("\tor ");  break;
                                case LNOT: code.append("\tnot "); break;
                                case  ROL: code.append("\trol "); break;
                                case  ROR: code.append("\tror "); break;
                            }
                            
                            if (command.as.instruction.as.math.op != LNOT) [[likely]]
                            {
                                code.append(fmt::format("{}, {}\n",
                                    cvt(command.as.instruction.as.math.op1),
                                    cvt(command.as.instruction.as.math.op2)));
                            }
                            else [[unlikely]]
                            {
                                code.append(fmt::format("{}\n",
                                    cvt(command.as.instruction.as.math.op1)));
                            }
                        } break;
                            
                        case MOVE:
                        {
                            using enum OperandType;
                            switch (command.as.instruction.as.move.op1.type)
                            {
                                case REG:
                                {
                                    if (command.as.instruction.as.move.op2.type != IMM) [[likely]]
                                    {
                                        using enum RegisterType;
                                        switch (command.as.instruction.as.move.op1.as.reg.location)
                                        {
                                            case R0: [[fallthrough]];
                                            case R1: [[fallthrough]];
                                            case R2: [[fallthrough]];
                                            case R3: [[fallthrough]];
                                            case RF: code.append("\tmvb "); break;
                                                
                                            case IP: ti::throw_error("Cannot move data directly into or out of the instruction pointer"); break;
                                        }
                                    }
                                    
                                    else [[unlikely]]
                                    {
                                        code.append("\tldb "); break;
                                    }
                                }
                                    
                                case MEM: code.append("\tstb "); break;
                                  
                                default: throw_error("Move instruction has invalid destination"); break;
                            }
                            
                            code.append(fmt::format("{}, {}\n",
                                cvt(command.as.instruction.as.math.op1),
                                cvt(command.as.instruction.as.math.op2)));
                        } break;
                            
                        case JMP:
                        {
                            using enum insn::Jmp::Condition;
                            switch (command.as.instruction.as.jmp.cond)
                            {
                                case JEZ: code.append("\tjez "); break;
                                case JCS: code.append("\tjcs "); break;
                            }
                            
                            code.append(fmt::format("{}\n", command.as.instruction.as.jmp.label));
                        } break;
                            
                        case STACK:
                        {
                            using enum insn::Stack::Direction;
                            switch (command.as.instruction.as.stack.dir)
                            {
                                case PUSH: code.append("\tpush "); break;
                                case  POP: code.append("\tpop ");  break;
                            }
                            
                            if (command.as.instruction.as.stack.op.type == OperandType::REG)
                            {
                                using enum RegisterType;
                                switch (command.as.instruction.as.stack.op.as.reg.location)
                                {
                                    case R0: code.append("r0\n");    break;
                                    case R1: code.append("r1\n");    break;
                                    case R2: code.append("r2\n");    break;
                                    case R3: code.append("r3\n");    break;
                                    case RF: code.append("flags\n"); break;
                                    case IP: code.append("ip\n");    break;
                                }
                            }
                        } break;
                            
                        case DEREF:
                        {
                            using enum insn::Deref::Operation;
                            switch (command.as.instruction.as.deref.op)
                            {
                                case AB_TO_A: code.append("\tderef r0-r1, r0\n"); break;
                                case CD_TO_C: code.append("\tderef r2-r3, r2\n"); break;
                            }
                        } break;
                    }
                } break;
                    
                case LABEL: code.append(fmt::format("@{}:\n", command.as.label.name)); break;
            }
        }
        
        return code;
    }
}
