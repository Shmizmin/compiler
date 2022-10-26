#include "Optimizer.hpp"

namespace
{
    void optimize_ast(ti::Optimizer& optimizer) noexcept
    {
        
    }
    
    void optimize_ir(ti::Optimizer& optimizer) noexcept
    {
        
    }
}


namespace ti
{
    void optimize(Optimizer& optimizer) noexcept
    {
        if (optimizer.optimize_ast) [[likely]]
        {
            optimize_ast(optimizer);
        }
        
        if (optimizer.optimize_ir) [[likely]]
        {
            optimize_ir(optimizer);
        }
    }
}
