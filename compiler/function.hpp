#ifndef function_hpp
#define function_hpp

#include <string>
#include <vector>

#include "types.hpp"

namespace ti
{
    struct Compiler;
    struct Statement;
    
    struct Argument
    {
        std::string name;
        CompleteType type;
    };
    
    struct Function
    {
        std::string name;
        CompleteType return_type;
        std::vector<Argument> arguments;
        Statement* body;
    };
    
    inline Function* compiling_function;
    
    void compile_function(Function*, Compiler&) noexcept;
}

#endif
