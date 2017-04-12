#include "Pass.h"

static llvm::RegisterPass<SEPass> SE("se", "SE Pass", false, false);
