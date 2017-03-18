#pragma once

namespace llvm {
    class Module;
    class Type;
}

class Instrumenter
{
public:
    void instrumentMain(llvm::Module* module);

private:
    llvm::Type* typeVoid(llvm::Module* module);
};
