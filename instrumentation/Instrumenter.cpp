#include "Instrumenter.h"

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>
#include "util/Demangler.h"

using namespace llvm;

void Instrumenter::instrumentMain(Module* module)
{
    Function* main = Demangler::get().getFunctionByDemangledName(module, "main");
    assert(main);

    BasicBlock& bb = main->getEntryBlock();
    Instruction* instruction = bb.getFirstNonPHI();

    Function* callback = cast<Function>(module->getOrInsertFunction("__se_init", this->typeVoid(module), nullptr));
    CallInst::Create(callback , {}, "", instruction);
}

Type* Instrumenter::typeVoid(Module* module)
{
    return Type::getVoidTy(module->getContext());
}
