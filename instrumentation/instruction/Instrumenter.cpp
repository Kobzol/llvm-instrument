#include "Instrumenter.h"

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/Debug.h>
#include "../util/Demangler.h"
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
    builder.CreateCall(this->functionBuilder.init(module));
}
void Instrumenter::instrumentStore(Module* module, StoreInst* store)
{
    IRBuilder<> builder(store);

    Value* dst = store->getPointerOperand();
    Value* src = store->getValueOperand();

    builder.CreateCall(this->functionBuilder.store(module), {
            CastInst::CreatePointerCast(dst, Types::int8(module)->getPointerTo(), "", store),
            Values::int64(module, dst->getType()->getPointerElementType()->getPrimitiveSizeInBits()),
            this->buildExpression(module, src, store)
    });
}
void Instrumenter::instrumentLoad(Module* module, LoadInst* load)
{
    IRBuilder<> builder(load);

    Value* src = load->getPointerOperand();
    if (auto* gep = dyn_cast<GetElementPtrInst>(src))
    {
        Value* buffer = gep->getPointerOperand();
        Value* operand = gep->getOperand(1);    // TODO more indices

        Value* indexExpression = this->buildExpression(module, operand, load);
        builder.CreateCall(this->functionBuilder.checkGEP(module), {
                CastInst::CreatePointerCast(buffer, Types::int8(module)->getPointerTo(), "", load),
                indexExpression
        });
    }
}
void Instrumenter::instrumentBranch(Module* module, BranchInst* branch)
{
    if (branch->isConditional())
    {
        BasicBlock* validLabel = branch->getSuccessor(0);
        BasicBlock* invalidLabel = branch->getSuccessor(1);

        IRBuilder<> builder(branch);
        builder.CreateCall(this->functionBuilder.branch(module), {
                this->buildExpression(module, branch->getCondition(), branch),
                branch->getCondition(),
                BlockAddress::get(branch->getFunction(), validLabel),
                BlockAddress::get(branch->getFunction(), invalidLabel)
        });
    }
}

Value* Instrumenter::buildExpression(Module* module, Value* value, Instruction* insertionPoint)
{
    ExprBuilder builder(insertionPoint);
    return builder.buildExpression(module, value);
}
