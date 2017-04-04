#pragma once

#include "z3++.h"

#include <cstdlib>

class Constraint
{
public:
    Constraint(z3::context* ctx, size_t size);

    size_t getSize() const;

    void setConstant(ssize_t constant, size_t size);

private:
    z3::context* ctx;
    z3::expr expression;

    size_t size;
};
