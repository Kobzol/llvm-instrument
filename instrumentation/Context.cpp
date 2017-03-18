#include "Context.h"

#include <llvm/IR/IntrinsicInst.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include <instruction/InstructionDispatcher.h>

void Context::handleModule(llvm::Module* module)
{
    this->instrumenter.instrumentMain(module);

    for (auto& fn : module->getFunctionList())
    {
        for (auto& block : fn.getBasicBlockList())
        {
            llvm::Instruction* inst = block.getFirstNonPHI();
            while (inst != nullptr)
            {
                InstructionDispatcher::get().dispatch(module, inst);
                inst = inst->getNextNode();
            }
        }
    }
}
