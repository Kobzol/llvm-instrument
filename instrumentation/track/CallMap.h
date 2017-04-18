#pragma once

#include <unordered_map>

namespace llvm {
    class CallInst;
    class Value;
}

class CallMap
{
public:
    void storeReturn(llvm::CallInst* call, llvm::Value* value);
    llvm::Value* getReturn(llvm::CallInst* call);

private:
    std::unordered_map<llvm::Value*, llvm::Value*> callMap;
};
