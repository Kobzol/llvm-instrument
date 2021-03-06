#pragma once

#include "z3++.h"

#include <cstdlib>

class Constraint
{
public:
    Constraint(z3::context* ctx, size_t size);
    ~Constraint();

    size_t getSize() const;

    virtual z3::expr createExpr() = 0;
    __attribute((noinline)) virtual void dump(int level = 0) = 0;

protected:
    z3::context* ctx;
    size_t size;
};
