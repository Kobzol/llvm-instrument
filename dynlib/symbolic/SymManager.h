#pragma once

#include <unordered_map>
#include "../memory/HeapBlock.h"
#include "Constraint.h"
#include "../../common/CmpType.h"
#include "../path/PathCondition.h"
#include "ICmp.h"

class SymManager
{
public:
    SymManager();

    void makeSymbolic(void* mem, size_t size);

    void* exprConst(size_t value, size_t size);
    void* exprLoad(void* address, size_t size);
    void* exprAdd(Constraint* op1, Constraint* op2, size_t size);
    void* exprICmp(Constraint* op1, Constraint* op2, CmpType conditionType);

    void store(void* address, size_t size, Constraint* constraint);
    void branch(ICmp* condition, bool concreteCondition, void* trueLabel, void* falseLabel);

private:
    Constraint* getConstraint(void* mem);
    size_t hashAddr(void* addr);

    z3::context ctx;
    std::unordered_map<size_t, Constraint*> memoryConstraints;
    PathCondition pathCondition;
};
