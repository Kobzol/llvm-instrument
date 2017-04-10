#pragma once

#include "Constraint.h"

class Constant : public Constraint
{
public:
    Constant(z3::context* ctx, size_t size, size_t value);

    z3::expr createExpr() override;

    void dump(int level) override;

private:
    size_t value;
};
