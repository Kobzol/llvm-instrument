#include "ExprBuilder.h"
#include "Instrumenter.h"
#include "Types.h"
#include "Values.h"

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/IRBuilder.h>

using namespace llvm;

ExprBuilder::ExprBuilder(llvm::Instruction* insertionPoint): insertionPoint(insertionPoint)
{

}

CallInst* ExprBuilder::buildExpression(Module* module, Value* value)
{
    if (auto* constant = dyn_cast<Constant>(value))
    {
        return this->buildConstant(module, constant);
    }
    else if (auto* binop = dyn_cast<BinaryOperator>(value))
    {
        return this->buildBinOp(binop);
    }

    assert(false);
    return nullptr;
}

CallInst* ExprBuilder::buildConstant(Module* module, Constant* constant)
{
    Type* type = constant->getType();
    assert(type->isIntegerTy());

    Function* constFn = this->functionBuilder.getExprConst(module);

    IRBuilder<> builder(this->insertionPoint);
    Value* castedValue = builder.CreateIntCast(constant, Types::int64(module), true);

    return builder.CreateCall(constFn, {
            castedValue,
            Values::int64(module, type->getPrimitiveSizeInBits())
    });
}

CallInst* ExprBuilder::buildBinOp(BinaryOperator* oper)
{
    assert(oper->getOpcode() == BinaryOperator::BinaryOps::Add);

    return nullptr;
}
