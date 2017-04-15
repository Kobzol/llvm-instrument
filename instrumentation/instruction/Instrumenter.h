#pragma once

#include <cstddef>
#include <llvm/IR/Instructions.h>

#include "Functions.h"

namespace llvm {
    class BranchInst;
    class Module;
    class Instruction;
    class LoadInst;
    class StoreInst;
    class Value;
}

class Instrumenter
{
public:
    void instrumentMain(llvm::Module* module);
    void instrumentStore(llvm::Module* module, llvm::StoreInst* store);
    void instrumentLoad(llvm::Module* module, llvm::LoadInst* load);
    void instrumentBranch(llvm::Module* module, llvm::BranchInst* branch);

    void instrumentAlloca(llvm::Module* module, llvm::AllocaInst* alloca);

private:
    llvm::Value* buildExpression(llvm::Module* module, llvm::Value* value, llvm::Instruction* insertionPoint);
    llvm::Function* getMainFunction(llvm::Module* module) const;

    Functions functionBuilder;

    void instrumentGlobals(llvm::Module* module, llvm::CallInst* initCall);
};
