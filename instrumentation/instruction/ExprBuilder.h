#pragma once

#include <llvm/IR/Constant.h>
#include <llvm/IR/InstrTypes.h>
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
    llvm::Instruction* insertionPoint;

    llvm::CallInst* buildConstant(llvm::Module* module, llvm::Constant* constant);
    llvm::CallInst* buildBinOp(llvm::BinaryOperator* oper);

    FunctionBuilder functionBuilder;
};
