#include "MemoryCompileAction.h"

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Tooling/Tooling.h>

MemoryCompileAction::MemoryCompileAction(std::unique_ptr<llvm::Module>* holder, llvm::LLVMContext& context): EmitLLVMOnlyAction(&context), holder(holder)
{

}

MemoryCompileAction::~MemoryCompileAction()
{
    *this->holder = this->takeModule();
    assert(this->holder->get());
}
