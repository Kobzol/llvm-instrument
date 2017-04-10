#pragma once

#include <unordered_map>
#include "../memory/HeapBlock.h"
#include "Constraint.h"
#include "../../common/CmpType.h"

class SymManager
{
public:
    void makeSymbolic(void* mem, size_t size);

    void* exprConst(size_t value, size_t size);
    void* exprLoad(void* address, size_t size);
    void* exprAdd(Constraint* op1, Constraint* op2, size_t size);
    void* exprICmp(Constraint* op1, Constraint* op2, CmpType conditionType);

    void store(void* address, size_t size, Constraint* constraint);

private:
    Constraint* getConstraint(void* mem);
    size_t hashAddr(void* addr);

    z3::context ctx;
    std::unordered_map<size_t, Constraint*> memoryConstraints;
};
