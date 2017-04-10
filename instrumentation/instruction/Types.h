#pragma once

namespace llvm {
    class Type;
    class Module;
}

class Types
{
public:
    static llvm::Type* voidType(llvm::Module* module);

    static llvm::Type* int8(llvm::Module* module);
    static llvm::Type* int64(llvm::Module* module);

    static llvm::Type* voidPtr(llvm::Module* module);
    static llvm::Type* int8Ptr(llvm::Module* module);
    static llvm::Type* int64Ptr(llvm::Module* module);
};
