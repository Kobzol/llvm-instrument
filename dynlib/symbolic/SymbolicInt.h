#pragma once

#include "Constraint.h"

class SymbolicInt : public Constraint
{
public:
    SymbolicInt(z3::context* ctx, size_t size, void* address);

    void dump(int level) override;

private:
    void* address;
};