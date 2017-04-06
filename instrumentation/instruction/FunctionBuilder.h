#pragma once

namespace llvm {
    class Function;
    class Module;
}

class FunctionBuilder
{
public:
    llvm::Function* getInitFunction(llvm::Module* module);
    llvm::Function* getStoreFunction(llvm::Module* module);

    llvm::Function* getPrintAddr(llvm::Module* module);

    llvm::Function* getExprConst(llvm::Module* module);
};
