#pragma once

#include <string>
#include <memory>
#include <cxxabi.h>
#include <cstring>

namespace llvm {
    class Function;
    class Instruction;
}

std::string demangleFunction(std::string name);

llvm::Instruction* getLastInstruction(llvm::Function* fn);
