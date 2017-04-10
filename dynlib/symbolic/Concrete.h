#pragma once

#include "Constraint.h"

class Concrete : public Constraint
{
public:
    Concrete(z3::context* ctx, size_t size, void* address);

    void dump(int level) override;

private:
    void* address;
};
