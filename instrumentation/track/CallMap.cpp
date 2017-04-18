#include "CallMap.h"

#include <llvm/IR/Instructions.h>

using namespace llvm;

void CallMap::storeReturn(CallInst* call, Value* value)
{
    this->callMap[call] = value;
}

Value* CallMap::getReturn(CallInst* call)
{
    return this->callMap[call];
}
