#pragma once

#include <llvm/IR/Constant.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instructions.h>
#include "Functions.h"

class Instrumenter;

namespace llvm {
    class BinaryOperator;
    class Module;
    class Value;
    class Function;
    class CallInst;
}

class ExprBuilder
{
public:
    ExprBuilder(llvm::Instruction* insertionPoint);

    llvm::Value* buildExpression(llvm::Module* module, llvm::Value* value);

private:
    llvm::Value* buildConstant(llvm::Module* module, llvm::Constant* constant);
    llvm::Value* buildBinOp(llvm::Module* module, llvm::BinaryOperator* oper);
    llvm::Value* buildLoad(llvm::Module* module, llvm::LoadInst* load);
    llvm::Value* buildIntegerCmp(llvm::Module* module, llvm::ICmpInst* instruction);
    llvm::Value* buildCall(llvm::Module* module, llvm::CallInst* call);
    llvm::Value* buildCast(llvm::Module* module, llvm::CastInst* cast);

    llvm::Instruction* insertionPoint;
    Functions functionBuilder;
};
