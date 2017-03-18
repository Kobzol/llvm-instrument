#include "Pass.h"

char SEPass::ID = 0;

SEPass::SEPass(): ModulePass(SEPass::ID)
{

}

bool SEPass::runOnModule(llvm::Module &m)
{
    this->context.handleModule(&m);
    return false;
}
