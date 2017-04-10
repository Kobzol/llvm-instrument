#include "Constant.h"
#include "../util/Logger.h"

Constant::Constant(z3::context* ctx, size_t size, size_t value): Constraint(ctx, size), value(value)
{

}

void Constant::dump(int level)
{
    Logger::log(level, "Constant %lu\n", this->value);
}

z3::expr Constant::createExpr()
{
    return this->ctx->int_val((unsigned long long) this->value);
}
