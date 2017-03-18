#pragma once

#include <clang/CodeGen/CodeGenAction.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/IR/Module.h>

namespace llvm {
    class LLVMContext;
}

class MemoryCompileAction : public clang::EmitLLVMOnlyAction
{
public:
    MemoryCompileAction(std::unique_ptr<llvm::Module>* holder, llvm::LLVMContext& ctx);

public:
    virtual ~MemoryCompileAction() override;

private:
    std::unique_ptr<llvm::Module>* holder;
};
