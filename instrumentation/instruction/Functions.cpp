#include "Functions.h"
#include "Types.h"

#include <llvm/Support/Casting.h>
#include <llvm/IR/Module.h>

using namespace llvm;

static std::string prefix = "__se_";

Function* Functions::init(Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("init"),
                                                      Types::voidType(module),
                                                      nullptr
    ));
}
Function* Functions::store(Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("store"),
                                                      Types::voidType(module),
                                                      Types::voidPtr(module),
                                                      Types::int64(module),
                                                      Types::voidPtr(module),
                                                      nullptr));
}

Function* Functions::printAddr(Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("addr"),
                                                      Types::voidType(module),
                                                      Types::voidPtr(module),
                                                      nullptr));
}

Function* Functions::exprConst(Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("expr_const"),
                                                      Types::voidPtr(module),
                                                      Types::int64(module),
                                                      Types::int64(module),
                                                      nullptr));
}
Function* Functions::exprLoad(Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("expr_load"),
                                                      Types::voidPtr(module),
                                                      Types::voidPtr(module),
                                                      Types::int64(module),
                                                      nullptr));
}
Function* Functions::exprAdd(Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("expr_add"),
                                                      Types::voidPtr(module),
                                                      Types::voidPtr(module),
                                                      Types::voidPtr(module),
                                                      Types::int64(module),
                                                      nullptr));
}
Function* Functions::exprICmp(Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("expr_icmp"),
                                                      Types::voidPtr(module),
                                                      Types::voidPtr(module),
                                                      Types::voidPtr(module),
                                                      Types::int64(module), nullptr));
}

Function* Functions::branch(Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("branch"),
                                                      Types::voidType(module),
                                                      Types::voidPtr(module),
                                                      Types::boolType(module),
                                                      Types::voidPtr(module),
                                                      Types::voidPtr(module),
                                                      nullptr));
}
Function* Functions::checkGEP(Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("checkGEP"),
                                                      Types::voidType(module),
                                                      Types::voidPtr(module),
                                                      Types::voidPtr(module),
                                                      nullptr));
}

llvm::Function* Functions::stackAlloc(llvm::Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("stackAlloc"),
                                                      Types::voidType(module),
                                                      Types::voidPtr(module),
                                                      Types::int64(module),
                                                      nullptr));
}

llvm::Function* Functions::stackDealloc(llvm::Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("stackDealloc"),
                                                      Types::voidType(module),
                                                      Types::voidPtr(module),
                                                      nullptr));
}

std::string Functions::getName(std::string functionName)
{
    return prefix + functionName;
}
