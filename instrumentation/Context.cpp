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
                this->instrument(module, inst);
                inst = inst->getNextNode();
            }
        }
    }

    module->dump();
}

void Context::instrument(Module* module, Instruction* instruction)
{
    if (auto* store = dyn_cast<StoreInst>(instruction))
    {
        this->instrumenter.instrumentStore(module, store);
    }
    else if (auto* load = dyn_cast<LoadInst>(instruction))
    {
        this->instrumenter.instrumentLoad(module, load);
    }
    else if (auto* br = dyn_cast<BranchInst>(instruction))
    {
        this->instrumenter.instrumentBranch(module, br);
    }
}
