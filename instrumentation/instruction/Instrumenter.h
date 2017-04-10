#pragma once

#include <cstddef>

#include "FunctionBuilder.h"

namespace llvm {
    class BranchInst;
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
    void instrumentBranch(llvm::Module* module, llvm::BranchInst* branch);

private:
    llvm::Value* buildStoreExpression(llvm::Module* module, llvm::Value* value, llvm::Instruction* insertionPoint);
    llvm::Value* buildConditionExpression(llvm::Module* module, llvm::BranchInst* branch);

    FunctionBuilder functionBuilder;
};
