#ifndef Central_hpp
#define Central_hpp

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
    
    //reformatted from https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
    template<typename... T>
    std::string format(const std::string& format, T&&... t)
    {
        int size_s = std::snprintf(nullptr, 0, format.c_str(), t...) + 1; // Extra space for '\0'
        
        if (size_s <= 0)
        {
            throw std::runtime_error("Error during formatting.");
        }
        
        auto size = static_cast<size_t>(size_s);
        
        std::unique_ptr<char[]> buf(new char[size]);
        
        std::snprintf(buf.get(), size, format.c_str(), t...);
        
        return std::string( buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    }

}

#endif /* Central_hpp */
