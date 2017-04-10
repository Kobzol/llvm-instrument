#include "SymbolicInt.h"
#include "../util/Logger.h"

SymbolicInt::SymbolicInt(z3::context* ctx, size_t size, void* address): Constraint(ctx, size), address(address)
{

}

void SymbolicInt::dump(int level)
{
    Logger::log(level, "SymbolicInt at %p\n", this->address);
}

z3::expr SymbolicInt::createExpr()
{
    return this->ctx->int_const(("0x" + std::to_string(reinterpret_cast<size_t>(this->address))).c_str());
}
