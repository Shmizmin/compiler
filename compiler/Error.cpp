#include "Error.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstdarg>

void ti::throw_error(const char* msg, ...) noexcept
{
    std::va_list args;
    va_start(args, msg);
    
    std::fprintf(stderr, "[Error]");
    std::vfprintf(stderr, msg, args);
    std::fprintf(stderr, "\nExiting...\n");
    
    std::exit(EXIT_FAILURE);
    
    va_end(args);
}

void ti::throw_warning(const char* msg, ...) noexcept
{
    std::va_list args;
    va_start(args, msg);
    
    std::vfprintf(stderr, msg, args);
    std::exit(EXIT_FAILURE);
    
    va_end(args);
}
