#ifndef Driver_hpp
#define Driver_hpp

#include <string>
#include <vector>

#include "Types.hpp"
#include "Function.hpp"
#include "Statement.hpp"
#include "Expression.hpp"

#include "location.hh"
#include "Parser.hpp"

class Driver;

#undef YY_DECL
#define YY_DECL yy::parser::symbol_type yylex(Driver& drv)

YY_DECL;


class Driver
{
public:
    Driver(void) noexcept;
    
public:
    int result;
    std::string file;
    
    bool trace_scanning, trace_parsing;
    
    yy::location location;

    int parse(const std::string& f);

    void scan_begin();
    void scan_end();
};

#endif
