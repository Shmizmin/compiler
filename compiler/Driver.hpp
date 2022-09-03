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

class driver;

#undef YY_DECL
#define YY_DECL yy::parser::symbol_type yylex(driver& drv)

//YY_DECL;
yy::parser::symbol_type yylex(driver& drv);


class driver
{
public:
    driver(void) noexcept;
    
public:
    int result;
    std::string file;
    
    bool trace_scanning, trace_parsing;
    
    yy::location location;
    
    
    ti::TypeVisibility active_visibility;
    ti::CompleteType active_type;
    
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
