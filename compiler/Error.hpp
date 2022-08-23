#ifndef Error_hpp
#define Error_hpp

namespace ti
{
    template<typename... T>
    [[noreturn]] void throw_error(const char* msg, T&&... t) noexcept;
    
    template<typename... T>
    void throw_warning(const char* msg, T&&... t) noexcept;
    
    template<typename... T>
    void write_log(const char* msg, T&&... t) noexcept;
}

#endif /* Error_hpp */
