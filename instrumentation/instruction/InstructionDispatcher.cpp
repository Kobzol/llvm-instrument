#include "InstructionDispatcher.h"
#include "Store.h"
#include "Call.h"

#include <llvm/IR/Instruction.h>

InstructionDispatcher::InstructionDispatcher()
{
    this->handlers[llvm::Instruction::Store] = std::make_unique<Store>();
    this->handlers[llvm::Instruction::Call] = std::make_unique<Call>();
}

void InstructionDispatcher::dispatch(llvm::Module* module, llvm::Instruction* instruction)
{
    unsigned int opcode = instruction->getOpcode();
    if (opcode < 256 && this->handlers[opcode])
    {
        this->handlers[opcode]->handle(module, instruction);
    }
}
