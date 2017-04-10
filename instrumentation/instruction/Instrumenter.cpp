#include "Instrumenter.h"

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/Debug.h>
#include "util/Demangler.h"
#include "ExprBuilder.h"
#include "Types.h"
#include "Values.h"

using namespace llvm;

void Instrumenter::instrumentMain(Module* module)
{
    Function* main = Demangler::get().getFunctionByDemangledName(module, "main");
    assert(main);

    BasicBlock& bb = main->getEntryBlock();
    Instruction* instruction = bb.getFirstNonPHI();

    IRBuilder<> builder(instruction);
    builder.CreateCall(this->functionBuilder.getInitFunction(module));
}

void Instrumenter::instrumentStore(Module* module, StoreInst* store)
{
    Value* dst = store->getPointerOperand();
    Value* src = store->getValueOperand();

    IRBuilder<> builder(store);
    builder.CreateCall(this->functionBuilder.getStoreFunction(module), {
            CastInst::CreatePointerCast(dst, Types::int8(module)->getPointerTo(), "", store),
            Values::int64(module, dst->getType()->getPointerElementType()->getPrimitiveSizeInBits()),
            this->buildStoreExpression(module, src, store)
    });
}

void Instrumenter::instrumentBranch(Module* module, BranchInst* branch)
{
    if (branch->isConditional())
    {
        BasicBlock* validLabel = branch->getSuccessor(0);
        BasicBlock* invalidLabel = branch->getSuccessor(1);

        IRBuilder<> builder(branch);
        builder.CreateCall(this->functionBuilder.getBranchFunction(module), {
                this->buildConditionExpression(module, branch),
                branch->getCondition(),
                BlockAddress::get(branch->getFunction(), validLabel),
                BlockAddress::get(branch->getFunction(), invalidLabel)
        });
    }
}

Value* Instrumenter::buildStoreExpression(Module* module, Value* value, Instruction* insertionPoint)
{
    ExprBuilder builder(insertionPoint);
    return builder.buildExpression(module, value);
}

Value* Instrumenter::buildConditionExpression(llvm::Module* module, llvm::BranchInst* branch)
{
    ExprBuilder builder(branch);
    return builder.buildExpression(module, branch->getCondition());
}
