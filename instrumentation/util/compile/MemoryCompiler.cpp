#include "MemoryCompiler.h"

#include <clang/Tooling/Tooling.h>
#include <llvm/IR/PassManager.h>

#include "MemoryCompileAction.h"

llvm::LLVMContext MemoryCompiler::CTX;

MemoryCompiler::MemoryCompiler()
{

}

std::unique_ptr<llvm::Module> MemoryCompiler::compile(const std::string& code)
{
    std::unique_ptr<llvm::Module> moduleHolder;
    clang::tooling::runToolOnCodeWithArgs(
            new MemoryCompileAction(&moduleHolder, MemoryCompiler::CTX),
            code,
            { "-g", "-std=c++14", "-O0", "-I/home/kobzol/libraries/llvm3.9-binaries/lib/clang/3.9.0/include" }
    );
    return moduleHolder;
}
