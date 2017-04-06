#include "FunctionBuilder.h"
#include "Types.h"

#include <llvm/Support/Casting.h>
#include <llvm/IR/Module.h>

using namespace llvm;

Function* FunctionBuilder::getInitFunction(Module* module)
{
    return cast<Function>(module->getOrInsertFunction("__se_init",
                                                      Types::voidType(module),
                                                      nullptr
    ));
}
Function* FunctionBuilder::getStoreFunction(Module* module)
{
    return cast<Function>(module->getOrInsertFunction("__se_store",
                                                      Types::voidType(module),
                                                      Types::int8(module)->getPointerTo(),
                                                      Types::int64(module),
                                                      Types::int8(module)->getPointerTo(),
                                                      Types::int64(module),
                                                      nullptr));
}

llvm::Function* FunctionBuilder::getPrintAddr(llvm::Module* module)
{
    return cast<Function>(module->getOrInsertFunction("__se_print_addr",
                                                      Types::voidType(module),
                                                      Types::int8(module)->getPointerTo(),
                                                      nullptr));
}

llvm::Function* FunctionBuilder::getExprConst(llvm::Module* module)
{
    return cast<Function>(module->getOrInsertFunction("__se_expr_const",
                                                      Types::int8(module)->getPointerTo(),
                                                      Types::int64(module),
                                                      Types::int64(module),
                                                      nullptr));
}
