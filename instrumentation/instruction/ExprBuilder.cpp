#include "ExprBuilder.h"
#include "Instrumenter.h"
#include "Types.h"
#include "Values.h"

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/IRBuilder.h>

#include "../../common/CmpType.h"
#include "../../dynlib/util/Logger.h"

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

ExprBuilder::ExprBuilder(llvm::Instruction* insertionPoint, CallMap* callMap)
        : insertionPoint(insertionPoint), callMap(callMap)
{

}

Value* ExprBuilder::buildExpression(Module* module, Value* value)
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
    else if (auto* call = dyn_cast<CallInst>(value))
    {
        return this->buildCall(module, call);
    }
    else if (auto* cast = dyn_cast<CastInst>(value))
    {
        return this->buildCast(module, cast);
    }
    else if (auto* arg = dyn_cast<Argument>(value))
    {
        return this->buildArgument(module, arg);
    }
    else if (auto* alloca = dyn_cast<AllocaInst>(value))
    {
        return this->buildAlloca(module, alloca);
    }

    std::cerr << "Unknown expression: " << std::endl;
    value->dump();
    exit(1);
    assert(false);
    return nullptr;
}

Value* ExprBuilder::buildConstant(Module* module, Constant* constant)
{
    Type* type = constant->getType();
    //assert(type->isIntegerTy());

    Function* constFn = this->functionBuilder.exprConst(module);

    IRBuilder<> builder(this->insertionPoint);
    Value* castedValue = builder.CreateIntCast(constant, Types::int64(module), true);

    return builder.CreateCall(constFn, {
            castedValue,
            Values::int64(module, type->getPrimitiveSizeInBits())
    });
}

Value* ExprBuilder::buildBinOp(Module* module, BinaryOperator* oper)
{
    assert(oper->getOpcode() == BinaryOperator::BinaryOps::Add);

    Function* addFn = this->functionBuilder.exprAdd(module);

    IRBuilder<> builder(this->insertionPoint);
    return builder.CreateCall(addFn, {
            this->buildExpression(module, oper->getOperand(0)),
            this->buildExpression(module, oper->getOperand(1)),
            Values::int64(module, oper->getType()->getPrimitiveSizeInBits())
    });
}

Value* ExprBuilder::buildLoad(Module* module, LoadInst* load)
{
    Value* src = load->getPointerOperand();
    return this->buildLoad(module, src, src->getType()->getPrimitiveSizeInBits());
}

Value* ExprBuilder::buildLoad(Module* module, Value* value, size_t size)
{
    Function* loadFn = this->functionBuilder.exprLoad(module);
    IRBuilder<> builder(this->insertionPoint);
    return builder.CreateCall(loadFn, {
            builder.CreateBitOrPointerCast(value, Types::voidPtr(module)),
            Values::int64(module, size) // TODO: 0 size
    });
}

Value* ExprBuilder::buildIntegerCmp(Module* module, ICmpInst* instruction)
{
    Function* cmpFn = this->functionBuilder.exprICmp(module);

    Value* op1 = this->buildExpression(module, instruction->getOperand(0));
    Value* op2 = this->buildExpression(module, instruction->getOperand(1));

    IRBuilder<> builder(this->insertionPoint);
    return builder.CreateCall(cmpFn, {
            op1,
            op2,
            Values::int64(module, static_cast<size_t>(getICmpType(instruction)))
    });
}

Value* ExprBuilder::buildCall(Module* module, CallInst* call)
{
    Value* returnValue = this->callMap->getReturn(call);
    if (returnValue != nullptr)
    {
        return returnValue;
    }
    else return ConstantPointerNull::get(Types::int8Ptr(module));
}

Value* ExprBuilder::buildCast(Module* module, CastInst* cast)
{
    return this->buildExpression(module, cast->getOperand(0));
}

Value* ExprBuilder::buildArgument(Module* module, Argument* argument)
{
    IRBuilder<> builder(this->insertionPoint);
    return builder.CreateCall(this->functionBuilder.getParameter(module), {
            Values::int64(module, argument->getArgNo())
    });
}

Value* ExprBuilder::buildAlloca(Module* module, AllocaInst* alloca)
{
    Value* src = alloca;
    return this->buildLoad(module, src, src->getType()->getPrimitiveSizeInBits());
}
