#ifndef driver_hpp
#define driver_hpp

#include <string>
#include <vector>

#include "parser.hh"

#define YY_DECL \
  yy::parser::symbol_type yylex (driver& drv)
// ... and declare it for the parser's sake.
YY_DECL;


class driver
{
public:
    driver(void) noexcept;
    
public:
    int result
    std::string file;
    
    bool trace_scanning, trace_parsing;
    
    yy::location location;
    
    
    


    int parse(const std::string& f);

    void scan_begin();
    void scan_end();
};

#endif
