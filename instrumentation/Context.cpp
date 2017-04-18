#include "Context.h"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>

using namespace llvm;

void Context::handleModule(Module* module)
{
    this->instrumenter.instrumentMain(module);

    for (auto& fn : module->getFunctionList())
    {
        for (auto& block : fn.getBasicBlockList())
        {
            Instruction* inst = block.getFirstNonPHI();
            while (inst != nullptr)
            {
                Instruction* next = inst->getNextNode();
                this->instrument(module, inst);
                inst = next;
            }
        }
    }

    module->dump();
}

void Context::instrument(Module* module, Instruction* instruction)
{
    this->instrumenter.instrument(module, instruction);
}
