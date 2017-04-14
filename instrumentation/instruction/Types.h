#pragma once

namespace llvm {
    class Type;
    class Module;
    class PointerType;
}

class Types
{
public:
    static llvm::Type* voidType(llvm::Module* module);

    static llvm::Type* int8(llvm::Module* module);
    static llvm::Type* int64(llvm::Module* module);
    static llvm::Type* boolType(llvm::Module* module);

    static llvm::PointerType* voidPtr(llvm::Module* module);
    static llvm::PointerType* int8Ptr(llvm::Module* module);
    static llvm::PointerType* int64Ptr(llvm::Module* module);
};
