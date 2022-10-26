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

#endif
