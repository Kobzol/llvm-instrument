#include "Util.h"

#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>

using namespace llvm;

std::string demangleFunction(std::string name)
{
    std::unique_ptr<char, void(*)(void*)> demangled = std::unique_ptr<char, void(*)(void*)>(
            abi::__cxa_demangle(name.c_str(), NULL, NULL, NULL),
            std::free
    );

    if (!demangled.get()) return "";

    size_t count = std::strlen(demangled.get());
    return std::string(demangled.get(), count);
}

Instruction* getLastInstruction(Function* fn)
{
    for (auto& block : fn->getBasicBlockList())
    {
        for (auto& inst : block.getInstList())
        {
            if (isa<ReturnInst>(inst))
            {
                return &inst;
            }
        }
    }

    return nullptr;
}
