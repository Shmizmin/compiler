#ifndef program_hpp
#define program_hpp

#include <string>
#include <vector>

namespace ti
{
    struct Function;
    class Compiler;
    
    struct Parameters
    {
        std::string file_name;
        bool optimize_ast, optimize_ir;
    };
    
    struct Program
    {
        std::vector<Function*> functions;
    };
    
    void compile_program(Program*, Compiler&, Parameters&) noexcept;
}

#endif
