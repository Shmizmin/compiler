#include "IR.hpp"
#include "Utility.hpp"

namespace
{
    
}


namespace ti
{
    std::string generate_commands(const std::vector<Command>& commands) noexcept
    {
        std::string code;
        
        for (const auto& command : commands)
        {
            using enum CommandType;
            
            switch (command.type)
            {
                case DIRECTIVE:
                {
                    using enum DirectiveType;
                    switch (command.as.directive.type)
                    {
                        case ORIGIN: code.append(ti::format(".org %u",       command.as.directive.as.origin.address));      break;
                        case BYTE:   code.append(ti::format(".byte $u",      command.as.directive.as.word.value));          break;
                        case WORD:   code.append(ti::format(".word %u",      command.as.directive.as.word.value));          break;
                        case ASCII:  code.append(ti::format(".ascii \"%s\"", command.as.directive.as.ascii.text.c_str()));  break;
                    }
                } break;
                    
                case INSTRUCTION:
                {
                    constexpr auto cvt = [&](Operand op) noexcept -> std::string
                    {
                        using enum OperandType;
                        switch (op.type)
                        {
                            case A:   return "r0";
                            case B:   return "r1";
                            case C:   return "r2";
                            case D:   return "r3";
                            case F:   return "flags";
                            case IP:  return "ip";
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
                                case ADC:  code.append("\tadc ");       break;
                                case SBB:  code.append("\tsbb %s, %s"); break;
                                case LAND: code.append("\tand %s, %s"); break;
                                case LOR:  code.append("\tor %s, %s");  break;
                                case LNOT: code.append("\tnot %s");     break;
                                case ROL:  code.append("\trol %s, %s"); break;
                                case ROR:  code.append("\tror %s, %s"); break;
                            }
                            
                            if (command.as.instruction.as.math.op != LNOT) [[likely]]
                            {
                                code.append(format("%s, %s\n",
                                    cvt(command.as.instruction.as.math.op1).c_str(),
                                    cvt(command.as.instruction.as.math.op2).c_str()));
                            }
                            else [[unlikely]]
                            {
                                code.append(format("%s\n",
                                    cvt(command.as.instruction.as.math.op1).c_str()));
                            }
                        } break;
                            
                        case MOVE:
                        {
                            using enum OperandType;
                            switch (command.as.instruction.as.move.op1.type)
                            {
                                case A: [[fallthrough]];
                                case B: [[fallthrough]];
                                case C: [[fallthrough]];
                                case D:
                                {
                                    switch (command.as.instruction.as.move.op2.type)
                                    {
                                        case   A: [[fallthrough]];
                                        case   B: [[fallthrough]];
                                        case   C: [[fallthrough]];
                                        case   D: code.append("\tmvb "); break;
                                        case IMM: code.append("\tldb "); break;
                                        
                                        default: throw_error("Move instruction has invalid source"); break;
                                    }
                                } break;
                                
                                case MEM: code.append("\tstb "); break;
                                  
                                default: throw_error("Move instruction has invalid destination"); break;
                            }
                            
                            code.append(format("%s, %s\n",
                                cvt(command.as.instruction.as.math.op1).c_str(),
                                cvt(command.as.instruction.as.math.op2).c_str()));
                        } break;
                            
                        case JMP:
                        {
                            using enum insn::Jmp::Condition;
                            switch (command.as.instruction.as.jmp.cond)
                            {
                                case JEZ: code.append("\tjez "); break;
                                case JCS: code.append("\tjcs "); break;
                            }
                            
                            code.append(command.as.instruction.as.jmp.label);
                        } break;
                            
                        case STACK
                        {
                            using enum insn::Stack::Direction;
                            using enum insn::Stack::Data;
                            switch (command.as.instruction.as.stack.dir)
                            {
                                case PUSH:
                                {
                                    switch (command.as.instruction.as.stack.data)
                                    {
                                        case 
                                    }
                                } break;
                                    
                                case POP:
                                {
                                    
                                } break;
                            }
                        } break;
                            
                        case DEREF
                        {
                            
                        } break;
                    }
                } break;
                    
                case LABEL: break;
            }
        }
        
        return code;
    }
}
