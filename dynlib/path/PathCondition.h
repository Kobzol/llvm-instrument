#pragma once

#include <z3++.h>

class PathCondition
{
public:
    PathCondition(z3::context* ctx);

    void addCondition(z3::expr expr);

    void dump();

private:
    z3::context* ctx;
    z3::expr condition;
};
