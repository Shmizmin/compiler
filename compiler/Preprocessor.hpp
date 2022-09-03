#ifndef Preprocessor_hpp
#define Preprocessor_hpp

#include <vector>
#include <string>
#include <string_view>

namespace cl
{
    struct Macro
    {
        std::string name;
        std::vector<std::string> args;
        std::string code;
    };
    
    struct Context
    {
        std::vector<Macro> defined_macros, invoked_macros;
    };
    
    void preprocess(std::string&, std::string_view) noexcept;
    
    void preprocess_include(Context&, std::string&, std::string_view) noexcept;
    void preprocess_definition(Context&, std::string&) noexcept;
    void preprocess_macro_definition(Context&, std::string&) noexcept;
    void preprocess_macro_invokation(Context&, std::string&) noexcept;
}

#endif /* Preprocessor_hpp */
