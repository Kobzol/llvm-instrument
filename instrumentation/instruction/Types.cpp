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
Type* Types::boolType(Module* module)
{
    return Type::getInt1Ty(module->getContext());
}

PointerType* Types::voidPtr(Module* module)
{
    return Types::int8Ptr(module);
}
PointerType* Types::int8Ptr(Module* module)
{
    return Types::int8(module)->getPointerTo();
}
PointerType* Types::int64Ptr(Module* module)
{
    return Types::int64(module)->getPointerTo();
}
