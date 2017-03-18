#pragma once

#include <string>

#include <llvm/IR/Module.h>
#include <Pass.h>

#include "util/Singleton.h"

class MemoryCompiler : public Singleton<MemoryCompiler>
{
    friend class Singleton<MemoryCompiler>;

public:
    static llvm::LLVMContext CTX;

    std::unique_ptr<llvm::Module> compile(const std::string& code);

private:
    MemoryCompiler();
};