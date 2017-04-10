#pragma once

#include "Constraint.h"

class Add : public Constraint
{
public:
    Add(z3::context* ctx, size_t size, Constraint* op1, Constraint* op2);

    z3::expr createExpr() override;

    void dump(int level) override;

private:
    Constraint* op1;
    Constraint* op2;
};
