#ifndef Driver_hpp
#define Driver_hpp

#include "Context.hpp"
#include "Function.hpp"
#include "Statement.hpp"
#include "Expression.hpp"

#include <string_view>

namespace ti
{
    struct Parameters
    {
        std::string file_name;
    };
    
    void generate_program(Program&, Paramters&) noexcept;
    void generate_function(Context&, Function&) noexcept;
    void generate_statement(Context&, Statement&) noexcept;
    void generate_expression(Context&, Expression&) noexcept;
}

#endif /* Driver_hpp */
