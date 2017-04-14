#pragma once

#include "Constraint.h"

class Concrete : public Constraint
{
public:
    Concrete(z3::context* ctx, size_t size, void* address);

    z3::expr createExpr() override;
    void dump(int level) override;

private:
    void* address;
};
