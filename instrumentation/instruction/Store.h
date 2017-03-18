#pragma once

#include "InstructionHandler.h"

class Store : public InstructionHandler
{
public:
    virtual void handle(llvm::Module* module, llvm::Instruction *instruction) override;
};
