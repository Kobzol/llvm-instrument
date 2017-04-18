#include "Instrumenter.h"

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include "../util/Demangler.h"
#include "ExprBuilder.h"
#include "Types.h"
#include "Values.h"
#include "../util/Util.h"

using namespace llvm;

void Instrumenter::instrument(Module* module, Instruction* instruction)
{
    if (auto* alloca = dyn_cast<AllocaInst>(instruction))
    {
        this->instrumentAlloca(module, alloca);
    }
    else if (auto* store = dyn_cast<StoreInst>(instruction))
    {
        this->instrumentStore(module, store);
    }
    else if (auto* load = dyn_cast<LoadInst>(instruction))
    {
        this->instrumentLoad(module, load);
    }
    else if (auto* br = dyn_cast<BranchInst>(instruction))
    {
        this->instrumentBranch(module, br);
    }
    else if (auto* ret = dyn_cast<ReturnInst>(instruction))
    {
        this->instrumentReturn(module, ret);
    }
    else if (auto* call = dyn_cast<CallInst>(instruction))
    {
        this->instrumentCall(module, call);
    }
}

void Instrumenter::instrumentMain(Module* module)
{
    Function* main = this->getMainFunction(module);
    assert(main);

    BasicBlock& bb = main->getEntryBlock();
    Instruction* instruction = bb.getFirstNonPHI();

    IRBuilder<> builder(instruction);
    CallInst* initCall = builder.CreateCall(this->functionBuilder.init(module));
    this->instrumentGlobals(module, initCall);
}

Function* Instrumenter::getMainFunction(Module* module) const
{
    return Demangler::get().getFunctionByDemangledName(module, "main");
}

void Instrumenter::instrumentStore(Module* module, StoreInst* store)
{
    IRBuilder<> builder(store);

    Value* dst = store->getPointerOperand();
    Value* src = store->getValueOperand();

    if (auto* gep = dyn_cast<GetElementPtrInst>(dst))
    {
        this->checkGEP(module, gep);
    }

    builder.CreateCall(this->functionBuilder.store(module), {
            builder.CreatePointerCast(dst, Types::int8Ptr(module)),
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
        this->checkGEP(module, gep);
    }
}

void Instrumenter::checkGEP(Module* module, GetElementPtrInst* gep)
{
    IRBuilder<> builder(gep);

    Value* buffer = gep->getPointerOperand();
    Value* operand = gep->getOperand(gep->getNumOperands() - 1);

    Value* indexExpression = this->buildExpression(module, operand, gep);
    builder.CreateCall(functionBuilder.checkGEP(module), {
            builder.CreatePointerCast(buffer, Types::int8Ptr(module)),
            indexExpression
    });
}

void Instrumenter::instrumentBranch(Module* module, BranchInst* branch)
{
    if (branch->isConditional())
    {
        BasicBlock* validLabel = branch->getSuccessor(0);
        BasicBlock* invalidLabel = branch->getSuccessor(1);

        IRBuilder<> builder(branch);
        Value* branchCall = builder.CreateCall(this->functionBuilder.branch(module), {
                this->buildExpression(module, branch->getCondition(), branch),
                branch->getCondition(),
                BlockAddress::get(branch->getFunction(), validLabel),
                BlockAddress::get(branch->getFunction(), invalidLabel)
        });

        branch->setCondition(branchCall);
    }
}

void Instrumenter::instrumentAlloca(Module* module, AllocaInst* alloca)
{
    Type* type = alloca->getAllocatedType();
    if (isa<ArrayType>(type))
    {
        if (alloca->isStaticAlloca())
        {
            Instruction* insertionPoint = alloca->getNextNode();
            IRBuilder<> builder(insertionPoint);

            Value* arrayAddress = builder.CreatePointerCast(alloca, Types::int8Ptr(module));
            builder.CreateCall(this->functionBuilder.stackAlloc(module), {
                    arrayAddress,
                    Values::int64(module, module->getDataLayout().getTypeAllocSize(type))
            });

            Function* fn = alloca->getFunction();
            Instruction* lastInstruction = getLastInstruction(fn);
            IRBuilder<> deallocBuilder(lastInstruction);
            deallocBuilder.CreateCall(this->functionBuilder.stackDealloc(module), {
                    arrayAddress
            });
        }
    }
}

void Instrumenter::instrumentGlobals(Module* module, CallInst* initCall)
{
    IRBuilder<> builder(initCall->getNextNode());

    for (auto& global : module->getGlobalList())
    {
        Type* type = global.getType()->getContainedType(0);
        if (isa<ArrayType>(type))
        {
            Value* arrayAddress = builder.CreatePointerCast(&global, Types::int8Ptr(module));
            builder.CreateCall(this->functionBuilder.globalVariable(module), {
                    arrayAddress,
                    Values::int64(module, module->getDataLayout().getTypeAllocSize(type))
            });
        }
    }
}

Value* Instrumenter::buildExpression(Module* module, Value* value, Instruction* insertionPoint)
{
    ExprBuilder builder(insertionPoint, &this->callMap);
    return builder.buildExpression(module, value);
}

void Instrumenter::instrumentReturn(Module* module, ReturnInst* ret)
{
    Value* retVal = ret->getReturnValue();
    if (retVal != nullptr)
    {
        IRBuilder<> builder(ret);
        builder.CreateCall(this->functionBuilder.setReturnValue(module), {
                this->buildExpression(module, retVal, ret)
        });
    }
}

void Instrumenter::instrumentCall(Module* module, CallInst* call)
{
    if (this->functionBuilder.isInstrumentedFn(call->getCalledFunction())) return;

    Instruction* insertionPoint = call->getNextNode();
    IRBuilder<> builder(insertionPoint);
    Value* retValue = builder.CreateCall(this->functionBuilder.getReturnValue(module));
    this->callMap.storeReturn(call, retValue);

    size_t argumentCount = call->getCalledFunction()->getArgumentList().size();
    std::vector<Value*> arguments;
    arguments.push_back(Values::int64(module, argumentCount));

    for (auto& arg: call->arg_operands())
    {
        arguments.push_back(this->buildExpression(module, arg, call));
    }

    IRBuilder<> callBuilder(call);
    callBuilder.CreateCall(this->functionBuilder.createFrame(module), arguments);
}
