#pragma once

#include "Singleton.h"

namespace llvm {
    class Function;
    class Module;
}

class Demangler : public Singleton<Demangler>
{
public:
    llvm::Function* getFunctionByDemangledName(llvm::Module* module, std::string name) const;
    std::string demangle(std::string name) const;
};
