#include "SymbolicInt.h"
#include "../util/Logger.h"

SymbolicInt::SymbolicInt(z3::context* ctx, size_t size, void* address): Constraint(ctx, size), address(address)
{

}

void SymbolicInt::dump(int level)
{
    Logger::log(level, "SymbolicInt at %p\n", this->address);
}
