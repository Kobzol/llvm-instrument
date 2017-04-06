#include "Values.h"
#include "Types.h"

#include <llvm/IR/Constants.h>

using namespace llvm;

Value* Values::int64(Module* module, size_t value)
{
    return ConstantInt::get(Types::int64(module), value, false);
}
