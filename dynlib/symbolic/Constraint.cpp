#include "Constraint.h"

Constraint::Constraint(z3::context* ctx, size_t size) : ctx(ctx), size(size), expression(*ctx)
{

}

size_t Constraint::getSize() const
{
    return this->size;
}

void Constraint::setConstant(ssize_t constant, size_t size)
{
    this->expression = this->ctx->bv_val((unsigned long long) constant, size);
}
