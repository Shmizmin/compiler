#include "Driver.hpp"
#include "Parser.hpp"

driver::driver(void) noexcept
  : trace_parsing(true),
    trace_scanning(true)
{
}

int driver::parse(const std::string &f)
{
    file = f;
    location.initialize(&file);
    scan_begin();
    yy::parser parser(*this);
    parser.set_debug_level(trace_parsing);
    int res = parser.parse();
    scan_end();
    return res;
}
