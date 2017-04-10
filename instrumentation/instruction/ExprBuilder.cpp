#include "ExprBuilder.h"
#include "Instrumenter.h"
#include "Types.h"
#include "Values.h"

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/IRBuilder.h>

#include "../../common/CmpType.h"

using namespace llvm;

static CmpType getICmpType(ICmpInst* icmp)
{
    switch (icmp->getPredicate())
    {
        case ICmpInst::Predicate::ICMP_EQ: return CmpType::Equal;
        case ICmpInst::Predicate::ICMP_NE: return CmpType::NotEqual;
        case ICmpInst::Predicate::ICMP_SGE: return CmpType::SGE;
        case ICmpInst::Predicate::ICMP_SLE: return CmpType::SLE;
        case ICmpInst::Predicate::ICMP_SGT: return CmpType::SGT;
        case ICmpInst::Predicate::ICMP_SLT: return CmpType::SLT;
        default:
            assert(false);
    }

    return CmpType::Equal;
}

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
        return this->buildBinOp(module, binop);
    }
    else if (auto* load = dyn_cast<LoadInst>(value))
    {
        return this->buildLoad(module, load);
    }
    else if (auto* icmp = dyn_cast<ICmpInst>(value))
    {
        return this->buildIntegerCmp(module, icmp);
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

CallInst* ExprBuilder::buildBinOp(Module* module, BinaryOperator* oper)
{
    assert(oper->getOpcode() == BinaryOperator::BinaryOps::Add);

    Function* addFn = this->functionBuilder.getExprAdd(module);

    IRBuilder<> builder(this->insertionPoint);
    return builder.CreateCall(addFn, {
            this->buildExpression(module, oper->getOperand(0)),
            this->buildExpression(module, oper->getOperand(1)),
            Values::int64(module, oper->getType()->getPrimitiveSizeInBits())
    });
}

CallInst* ExprBuilder::buildLoad(Module* module, LoadInst* load)
{
    Function* loadFn = this->functionBuilder.getExprLoad(module);

    IRBuilder<> builder(this->insertionPoint);
    return builder.CreateCall(loadFn, {
            builder.CreateBitOrPointerCast(load->getPointerOperand(), Types::voidPtr(module)),
            Values::int64(module, load->getPointerOperand()->getType()->getPrimitiveSizeInBits()) // TODO: 0 size
    });
}

CallInst* ExprBuilder::buildIntegerCmp(llvm::Module* module, llvm::ICmpInst* instruction)
{
    Function* cmpFn = this->functionBuilder.getExprICmp(module);

    Value* op1 = this->buildExpression(module, instruction->getOperand(0));
    Value* op2 = this->buildExpression(module, instruction->getOperand(1));

    IRBuilder<> builder(this->insertionPoint);
    return builder.CreateCall(cmpFn, {
            op1,
            op2,
            Values::int64(module, static_cast<size_t>(getICmpType(instruction)))
    });
}
