#pragma once

#include <memory>
#include <vector>

#include "instruction/Instrumenter.h"

namespace llvm {
    class Module;
    class Instruction;
}

class Context
{
public:
    void handleModule(llvm::Module* module);

private:
    void instrument(llvm::Module* module, llvm::Instruction* instruction);

    Instrumenter instrumenter;
};
