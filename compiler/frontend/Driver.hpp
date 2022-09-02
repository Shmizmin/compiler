#ifndef Driver_hpp
#define Driver_hpp

#include <string>
#include <vector>

#include "../Function.hpp"
#include "../Statement.hpp"
#include "../Expression.hpp"

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
    
    
    std::vector<ti::Statement*> statement_queue;
    std::vector<ti::Function> definition_queue;
    std::vector<ti::Argument> fdecl_args_queue;
    std::vector<ti::Expression*> fcall_args_queue;

    std::vector<ti::Variable*> var_decl_queue;


    int parse(const std::string& f);

    void scan_begin();
    void scan_end();
};

#endif
