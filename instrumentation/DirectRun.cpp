#include <fstream>
#include <sstream>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/Utils/UnifyFunctionExitNodes.h>
#include <llvm/IR/IRPrintingPasses.h>

#include "util/compile/MemoryCompiler.h"

using namespace llvm;

std::string readFile(const char* path)
{
    std::fstream fs(path);
    assert(fs.is_open());

    std::stringstream buffer;
    buffer << fs.rdbuf();

    return buffer.str();
}

void transformModule(Module* module)
{
    llvm::legacy::FunctionPassManager fnManager(module);
    fnManager.add(new UnifyFunctionExitNodes());
    fnManager.doInitialization();
    for (auto& fn : module->getFunctionList())
    {
        fnManager.run(fn);
    }
    fnManager.doFinalization();

    llvm::legacy::PassManager manager;
    manager.add(new SEPass());
    manager.run(*module);
}

int main(int argc, char** argv)
{
    std::string code = readFile(argv[1]);

    std::unique_ptr<Module> module = MemoryCompiler::get().compile(code);
    transformModule(module.get());

    return 0;
}
