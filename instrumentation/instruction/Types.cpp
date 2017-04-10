#include "Types.h"

#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

using namespace llvm;

Type* Types::voidType(Module* module)
{
    return Type::getVoidTy(module->getContext());
}

Type* Types::int8(Module* module)
{
    return Type::getInt8Ty(module->getContext());
}

Type* Types::int64(Module* module)
{
    return Type::getInt64Ty(module->getContext());
}

Type* Types::voidPtr(Module* module)
{
    return Types::int8Ptr(module);
}
Type* Types::int8Ptr(Module* module)
{
    return Types::int8(module)->getPointerTo();
}
Type* Types::int64Ptr(Module* module)
{
    return Types::int64(module)->getPointerTo();
}
