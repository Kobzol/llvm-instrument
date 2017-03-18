#pragma once

#include <memory>
#include <vector>

#include "instruction/InstructionHandler.h"

class Call : public InstructionHandler
{
public:
    virtual void handle(llvm::Module* module, llvm::Instruction* instruction) override;
};
