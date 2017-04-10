#pragma once

#include "Constraint.h"

class SymbolicInt : public Constraint
{
public:
    SymbolicInt(z3::context* ctx, size_t size, void* address);

    z3::expr createExpr() override;

    void dump(int level) override;

private:
    void* address;
};