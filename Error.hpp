#ifndef Error_hpp
#define Error_hpp

namespace ti
{
    void throw_error(const char* msg, ...) noexcept;
    void throw_warning(const char* msg, ...) noexcept;
}

#endif /* Error_hpp */
