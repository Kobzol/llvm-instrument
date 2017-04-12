#include "ICmp.h"
#include "../util/Logger.h"

ICmp::ICmp(z3::context* ctx, Constraint* op1, Constraint* op2, CmpType cmpType)
    : Constraint(ctx, op1->getSize()), op1(op1), op2(op2), cmpType(cmpType)
{

}

void ICmp::dump(int level)
{
    Logger::log(level, "Cmp %d\n", this->cmpType);
    this->op1->dump(level + 1);
    this->op2->dump(level + 1);
}

z3::expr ICmp::createExpr()
{
    return this->createComparison(this->op1->createExpr(), this->op2->createExpr());
}

z3::expr ICmp::createComparison(const z3::expr& expr1, const z3::expr& expr2)
{
    switch (this->cmpType)
    {
        case CmpType::Equal: return expr1 == expr2;
        case CmpType::NotEqual: return expr1 != expr2;
        case CmpType::SLE: return expr1 <= expr2;
        case CmpType::SLT: return expr1 < expr2;
        case CmpType::SGE: return expr1 >= expr2;
        case CmpType::SGT: return expr1 > expr2;
    }
}
