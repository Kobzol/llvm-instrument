#pragma once

#include <cstddef>

#include "FunctionBuilder.h"

namespace llvm {
    class Module;
    class Instruction;
    class StoreInst;
    class Value;
}

class Instrumenter
{
public:
    void instrumentMain(llvm::Module* module);
    void instrumentStore(llvm::Module* module, llvm::StoreInst* store);

private:
    llvm::Value* buildStoreExpression(llvm::Module* module, llvm::Value* value, llvm::Instruction* insertionPoint);

    FunctionBuilder functionBuilder;
};
