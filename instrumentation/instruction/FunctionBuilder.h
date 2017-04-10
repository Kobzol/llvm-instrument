#pragma once

#include <string>

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
    llvm::Function* getExprLoad(llvm::Module* module);
    llvm::Function* getExprAdd(llvm::Module* module);
    llvm::Function* getExprICmp(llvm::Module* module);

    llvm::Function* getBranchFunction(llvm::Module* module);

private:
    std::string getName(std::string functionName);
};
