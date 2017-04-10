#include "Constraint.h"

Constraint::Constraint(z3::context* ctx, size_t size) : ctx(ctx), size(size)
{

}

Constraint::~Constraint()
{

}

size_t Constraint::getSize() const
{
    return this->size;
}

z3::expr Constraint::createExpr()
{
    return z3::expr(*this->ctx);
}
