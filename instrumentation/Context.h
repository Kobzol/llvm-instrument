#pragma once

#include <memory>
#include <vector>

#include <llvm/IR/Module.h>
#include "Instrumenter.h"

class Context
{
public:
    void handleModule(llvm::Module* module);

private:
    Instrumenter instrumenter;
};
