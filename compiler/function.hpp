#ifndef function_hpp
#define function_hpp

#include <string>
#include <vector>

#include "types.hpp"

namespace ti
{
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
    
    struct Program
    {
        std::vector<Function*> functions;
    };
    
    void compile_function(Function*, CommonArgs&) noexcept;
}

#endif
