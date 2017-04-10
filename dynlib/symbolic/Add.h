#pragma once

#include "Constraint.h"

class Add : public Constraint
{
public:
    Add(z3::context* ctx, size_t size, Constraint* op1, Constraint* op2);

    void dump(int level) override;

private:
    Constraint* op1;
    Constraint* op2;
};
