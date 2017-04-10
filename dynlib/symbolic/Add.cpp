#include "Add.h"
#include "../util/Logger.h"

Add::Add(z3::context* ctx, size_t size, Constraint* op1, Constraint* op2)
        : Constraint(ctx, size), op1(op1), op2(op2)
{

}

void Add::dump(int level)
{
    Logger::log(level, "Add\n");
    this->op1->dump(level + 1);
    this->op2->dump(level + 1);
}

z3::expr Add::createExpr()
{
    return this->op1->createExpr() + this->op2->createExpr();
}
