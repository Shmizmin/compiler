#ifndef central_hpp
#define central_hpp

#include "compiler.hpp"
#include "function.hpp"
#include "statement.hpp"
#include "expression.hpp"

#include <string>

namespace ti
{
    struct Parameters
    {
        std::string file_name;
        bool optimize_ast, optimize_ir;
    };
    
    void compile_program(Program&, Parameters&) noexcept;
    void compile_function(Compiler&, Function&) noexcept;
}

#endif
