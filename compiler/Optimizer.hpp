#ifndef Optimizer_hpp
#define Optimizer_hpp

namespace ti
{
    struct Optimizer
    {
        bool optimize_ast, optimize_ir;
    };
    
    void optimize(Optimizer&) noexcept;
}

#endif
