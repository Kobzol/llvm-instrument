#pragma once

#include <unordered_map>
#include "../memory/MemoryBlock.h"
#include "Constraint.h"
#include "../../common/CmpType.h"
#include "../path/PathCondition.h"
#include "ICmp.h"
#include "../memory/MemoryManager.h"

class SymManager
{
public:
    SymManager();

    void makeSymbolic(void* mem, size_t size);

    Constraint* exprConst(size_t value, size_t size);
    Constraint* exprLoad(void* address, size_t size);
    Constraint* exprAdd(Constraint* op1, Constraint* op2, size_t size);
    Constraint* exprICmp(Constraint* op1, Constraint* op2, CmpType conditionType);

    void store(void* address, size_t size, Constraint* constraint);
    void branch(ICmp* condition, bool concreteCondition, void* trueLabel, void* falseLabel);

    void checkGEP(MemoryManager* heapManager, void* address, Constraint* indexer, const char* location);

private:
    Constraint* getConstraint(void* mem);
    size_t hashAddr(void* addr);

    z3::context ctx;
    std::unordered_map<size_t, Constraint*> memoryConstraints;
    PathCondition pathCondition;
};
