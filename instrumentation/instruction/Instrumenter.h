#pragma once

#include <cstddef>

#include "Functions.h"
#include "../track/CallMap.h"

namespace llvm {
    class AllocaInst;
    class BranchInst;
    class CallInst;
    class GetElementPtrInst;
    class Instruction;
    class LoadInst;
    class Module;
    class ReturnInst;
    class StoreInst;
    class Value;
}

class Instrumenter
{
public:
    void instrument(llvm::Module* module, llvm::Instruction* instruction);

    void instrumentMain(llvm::Module* module);

private:
    llvm::Value* buildExpression(llvm::Module* module, llvm::Value* value, llvm::Instruction* insertionPoint);
    llvm::Function* getMainFunction(llvm::Module* module) const;

    void instrumentStore(llvm::Module* module, llvm::StoreInst* store);
    void instrumentLoad(llvm::Module* module, llvm::LoadInst* load);
    void instrumentBranch(llvm::Module* module, llvm::BranchInst* branch);
    void instrumentAlloca(llvm::Module* module, llvm::AllocaInst* alloca);
    void instrumentReturn(llvm::Module* module, llvm::ReturnInst* ret);
    void instrumentCall(llvm::Module* module, llvm::CallInst* call);
    void instrumentGlobals(llvm::Module* module, llvm::CallInst* initCall);
    void checkGEP(llvm::Module* module, llvm::GetElementPtrInst* gep);

    llvm::Value* getLocation(llvm::Module* module, llvm::Instruction* instruction);

    CallMap callMap;
    Functions functionBuilder;
};
