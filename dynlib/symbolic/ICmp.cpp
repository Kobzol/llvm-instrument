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
