#ifndef Function_hpp
#define Function_hpp

#include <string>
#include <vector>
#include <compare>

#include "Statement.hpp"
#include "Types.hpp"

namespace ti
{
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
        std::vector<Function> functions;
    };
}

#endif /* Function_hpp */
