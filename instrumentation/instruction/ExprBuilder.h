#pragma once

#include "Functions.h"
#include "../track/CallMap.h"

class Instrumenter;

namespace llvm {
    class AllocaInst;
    class Argument;
    class BinaryOperator;
    class CallInst;
    class CastInst;
    class Constant;
    class ICmpInst;
    class Instruction;
    class LoadInst;
    class Module;
    class Value;
}

class ExprBuilder
{
public:
    ExprBuilder(llvm::Instruction* insertionPoint, CallMap* callMap);

    llvm::Value* buildExpression(llvm::Module* module, llvm::Value* value);

private:
    llvm::Value* buildConstant(llvm::Module* module, llvm::Constant* constant);
    llvm::Value* buildBinOp(llvm::Module* module, llvm::BinaryOperator* oper);
    llvm::Value* buildLoad(llvm::Module* module, llvm::LoadInst* load);
    llvm::Value* buildLoad(llvm::Module* module, llvm::Value* value, size_t size);
    llvm::Value* buildIntegerCmp(llvm::Module* module, llvm::ICmpInst* instruction);
    llvm::Value* buildCall(llvm::Module* module, llvm::CallInst* call);
    llvm::Value* buildCast(llvm::Module* module, llvm::CastInst* cast);
    llvm::Value* buildArgument(llvm::Module* module, llvm::Argument* argument);
    llvm::Value* buildAlloca(llvm::Module* module, llvm::AllocaInst* alloca);

    llvm::Instruction* insertionPoint;
    CallMap* callMap;
    Functions functionBuilder;
};
