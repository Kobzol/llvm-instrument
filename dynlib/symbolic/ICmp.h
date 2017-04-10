#pragma once

#include "Constraint.h"
#include "../../common/CmpType.h"

class ICmp : public Constraint
{
public:
    ICmp(z3::context* ctx, Constraint* op1, Constraint* op2, CmpType cmpType);

    z3::expr createExpr() override;

    void dump(int level = 0) override;

private:
    Constraint* op1;
    Constraint* op2;
    CmpType cmpType;
};
