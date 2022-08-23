#ifndef Driver_hpp
#define Driver_hpp

#include "Context.hpp"
#include "Function.hpp"
#include "Statement.hpp"
#include "Expression.hpp"

#include <string>

namespace ti
{
    struct Parameters
    {
        std::string file_name;
    };
    
    void generate_program(Program&, Parameters&) noexcept;
    void generate_function(Context&, Function&) noexcept;
    void generate_statement(Context&, Function&, Statement*) noexcept;
    void generate_expression(Context&, Function&, Expression*) noexcept;
    
    template<typename... Args>
    std::string format(const std::string& format, Args&&... args);
}

#endif /* Driver_hpp */
