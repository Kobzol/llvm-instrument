#pragma once

#include <z3++.h>

class PathCondition
{
public:
    PathCondition(z3::context* ctx);

    void addCondition(z3::expr expr);

    void dump();

    bool isSatisfiable(const z3::expr& expr, const z3::expr& indexer, z3::expr& model);

private:
    z3::context* ctx;
    z3::expr condition;
};
