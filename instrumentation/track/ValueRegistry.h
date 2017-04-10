#pragma once

#include <unordered_map>

namespace llvm {
    class Value;
}

class ValueRegistry
{
public:
    size_t getId(llvm::Value* value);

private:
    void insertId(llvm::Value* value);
    bool hasId(llvm::Value* value) const;

    size_t hashId(llvm::Value* value) const;

    std::unordered_map<size_t, size_t> map;
};
