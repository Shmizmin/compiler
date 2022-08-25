#ifndef Error_hpp
#define Error_hpp

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

namespace
{
    void log_internal(const char* head, const char* tail, const char* msg, ...) noexcept
    {
        std::va_list args;
        va_start(args, msg);
        
        std::fprintf(stderr, "%s", head);
        std::vfprintf(stderr, msg, args);
        std::fprintf(stderr, "%s", tail);
        
        va_end(args);
    }
}

namespace ti
{
    template<typename... T>
    [[noreturn]] void throw_error(const char* msg, T&&... t) noexcept
    {
        ::log_internal("[Error]", "\nExiting...\n", msg, t...);
        
        std::exit(EXIT_FAILURE);
    }

    template<typename... T>
    void throw_warning(const char* msg, T&&... t) noexcept
    {
        ::log_internal("[Warning]", "", msg, t...);
    }

    template<typename... T>
    void write_log(const char* msg, T&&... t) noexcept
    {
        ::log_internal("[Log]", "", msg, t...);
    }

}

#endif /* Error_hpp */
