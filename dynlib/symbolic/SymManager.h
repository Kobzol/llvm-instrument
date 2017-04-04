#pragma once

#include <unordered_map>
#include "../memory/HeapBlock.h"
#include "Constraint.h"

class SymManager
{
public:
    void makeSymbolic(void* mem, size_t size);
    void storeConst(void* memDst, size_t memSize, ssize_t constant, size_t constantSize);

private:
    Constraint* getConstraint(void* mem);

    z3::context ctx;
    std::unordered_map<size_t, Constraint> memoryConstraints;
};
