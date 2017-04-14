#pragma once

#include <string>

namespace llvm {
    class Function;
    class Module;
}

class Functions
{
public:
    llvm::Function* init(llvm::Module* module);
    llvm::Function* store(llvm::Module* module);

    llvm::Function* printAddr(llvm::Module* module);

    llvm::Function* exprConst(llvm::Module* module);
    llvm::Function* exprLoad(llvm::Module* module);
    llvm::Function* exprAdd(llvm::Module* module);
    llvm::Function* exprICmp(llvm::Module* module);

    llvm::Function* branch(llvm::Module* module);

    llvm::Function* checkGEP(llvm::Module* module);

private:
    std::string getName(std::string functionName);
};
