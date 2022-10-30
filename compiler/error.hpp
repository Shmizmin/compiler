#ifndef error_hpp
#define error_hpp

#include <cstdlib>
#include <iostream>
#include <fmt/format.h>

namespace ti
{
    [[noreturn]] inline void throw_error(std::string&& msg) noexcept
    {
        std::cerr << fmt::format("[Error] {}\n", std::move(msg));
        std::exit(EXIT_FAILURE);
    }

    inline void throw_warning(std::string&& msg) noexcept
    {
        std::cerr << fmt::format("[Warning] {}\n", std::move(msg));
    }

    inline void write_log(std::string&& msg) noexcept
    {
        std::clog << fmt::format("[Log] {}\n", std::move(msg));
    }

}

#endif
