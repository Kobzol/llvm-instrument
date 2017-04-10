#include "FunctionBuilder.h"
#include "Types.h"

#include <llvm/Support/Casting.h>
#include <llvm/IR/Module.h>

using namespace llvm;

static std::string prefix = "__se_";

Function* FunctionBuilder::getInitFunction(Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("init"),
                                                      Types::voidType(module),
                                                      nullptr
    ));
}
Function* FunctionBuilder::getStoreFunction(Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("store"),
                                                      Types::voidType(module),
                                                      Types::voidPtr(module),
                                                      Types::int64(module),
                                                      Types::voidPtr(module),
                                                      nullptr));
}

llvm::Function* FunctionBuilder::getPrintAddr(llvm::Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("addr"),
                                                      Types::voidType(module),
                                                      Types::voidPtr(module),
                                                      nullptr));
}

llvm::Function* FunctionBuilder::getExprConst(llvm::Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("expr_const"),
                                                      Types::voidPtr(module),
                                                      Types::int64(module),
                                                      Types::int64(module),
                                                      nullptr));
}
llvm::Function* FunctionBuilder::getExprLoad(llvm::Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("expr_load"),
                                                      Types::voidPtr(module),
                                                      Types::voidPtr(module),
                                                      Types::int64(module),
                                                      nullptr));
}
llvm::Function* FunctionBuilder::getExprAdd(llvm::Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("expr_add"),
                                                      Types::voidPtr(module),
                                                      Types::voidPtr(module),
                                                      Types::voidPtr(module),
                                                      Types::int64(module),
                                                      nullptr));
}
llvm::Function* FunctionBuilder::getExprICmp(llvm::Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("expr_icmp"),
                                                 Types::voidPtr(module),
                                                 Types::voidPtr(module),
                                                 Types::voidPtr(module),
                                                 Types::int64(module),
                                                 nullptr));
}

llvm::Function* FunctionBuilder::getBranchFunction(llvm::Module* module)
{
    return cast<Function>(module->getOrInsertFunction(this->getName("branch"),
                                                      Types::voidType(module),
                                                      Types::voidPtr(module),
                                                      Types::voidPtr(module),
                                                      Types::voidPtr(module),
                                                      nullptr));
}

std::string FunctionBuilder::getName(std::string functionName)
{
    return prefix + functionName;
}
