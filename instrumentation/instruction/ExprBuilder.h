#pragma once

#include <llvm/IR/Constant.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instructions.h>
#include "FunctionBuilder.h"

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

    llvm::CallInst* buildExpression(llvm::Module* module, llvm::Value* value);

private:
    llvm::CallInst* buildConstant(llvm::Module* module, llvm::Constant* constant);
    llvm::CallInst* buildBinOp(llvm::Module* module, llvm::BinaryOperator* oper);
    llvm::CallInst* buildLoad(llvm::Module* module, llvm::LoadInst* load);
    llvm::CallInst* buildIntegerCmp(llvm::Module* module, llvm::ICmpInst* instruction);

    llvm::Instruction* insertionPoint;
    FunctionBuilder functionBuilder;
};
