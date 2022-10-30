#ifndef function_hpp
#define function_hpp

#include <string>
#include <vector>

#include "types.hpp"

namespace ti
{
    struct Statement;
    struct RegisterAllocation;
    
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
    
    void compile_function(Function*, Compiler&) noexcept;
}

#endif
