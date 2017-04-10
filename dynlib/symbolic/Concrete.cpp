#include "Concrete.h"
#include "../util/Logger.h"

Concrete::Concrete(z3::context* ctx, size_t size, void* address): Constraint(ctx, size), address(address)
{

}

void Concrete::dump(int level)
{
    Logger::log(level, "Concrete at %p", this->address);
}
