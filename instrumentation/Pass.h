#pragma once

#include <llvm/Pass.h>

#include "Context.h"

struct SEPass : public llvm::ModulePass
{
    static char ID;

    SEPass();

    virtual bool runOnModule(llvm::Module& m) override;

private:
    Context context;
};
