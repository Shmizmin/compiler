#ifndef Central_hpp
#define Central_hpp

#include "Context.hpp"
#include "Function.hpp"
#include "Statement.hpp"
#include "Expression.hpp"

#include <string>
#include <memory>

namespace ti
{
    struct Parameters
    {
        std::string file_name;
        bool optimize_ast, optimize_ir;
    };
    
    void compile_program(Program&, Parameters&) noexcept;
    void compile_function(Context&, Function&) noexcept;
}

#endif
