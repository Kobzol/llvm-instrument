#pragma once

namespace llvm {
    class Instruction;
    class Module;
}

class InstructionHandler
{
public:
    virtual ~InstructionHandler();

    virtual void handle(llvm::Module* module, llvm::Instruction* instruction) = 0;
};
