#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/DynamicLibrary.h>
#include <iostream>

#include "Interpret.h"

int main(int argc, char** argv)
{
    llvm::LLVMContext context;
    llvm::SMDiagnostic diag;

    /*LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();*/

    std::unique_ptr<llvm::Module> module = llvm::parseIRFile(argv[1], diag, context);
    module->materializeAll();

    llvm::Function* entry = module->getFunction("main");

    std::string error;
    sys::DynamicLibrary::LoadLibraryPermanently(nullptr, &error);

    llvm::ExecutionEngine* engine = Interpret::create(std::move(module), &error);
    engine->runStaticConstructorsDestructors(false);
    engine->runFunctionAsMain(entry, {}, nullptr);
    engine->runStaticConstructorsDestructors(true);

    return 0;
}
