#pragma once

#include <cstddef>

namespace llvm {
    class Value;
    class Module;
}

class Values
{
public:
    static llvm::Value* int64(llvm::Module* module, size_t value);
};
