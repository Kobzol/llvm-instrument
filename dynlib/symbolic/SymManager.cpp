#include "SymManager.h"
#include "Constant.h"
#include "Concrete.h"
#include "Add.h"
#include "SymbolicInt.h"
#include "ICmp.h"

SymManager::SymManager(): pathCondition(&this->ctx)
{

}

void SymManager::makeSymbolic(void* mem, size_t size)
{
    size_t addr = reinterpret_cast<size_t>(mem);
    this->memoryConstraints[addr] = new SymbolicInt(&this->ctx, size, mem);
}

Constraint* SymManager::getConstraint(void* mem)
{
    auto it = this->memoryConstraints.find(this->hashAddr(mem));
    if (it == this->memoryConstraints.end()) return nullptr;

    return it->second;
}

void* SymManager::exprConst(size_t value, size_t size)
{
    return new Constant(&this->ctx, size, value);
}

void* SymManager::exprLoad(void* address, size_t size)
{
    Constraint* constraint = this->getConstraint(address);
    if (constraint != nullptr) return constraint;

    return new Concrete(&this->ctx, size, address);
}

void* SymManager::exprAdd(Constraint* op1, Constraint* op2, size_t size)
{
    return new Add(&this->ctx, size, op1, op2);
}

size_t SymManager::hashAddr(void* addr)
{
    return reinterpret_cast<size_t>(addr);
}

void SymManager::store(void* address, size_t size, Constraint* constraint)
{
    //assert(size == constraint->getSize());
    size_t addr = reinterpret_cast<size_t>(address);
    this->memoryConstraints[addr] = constraint;
}

void* SymManager::exprICmp(Constraint* op1, Constraint* op2, CmpType conditionType)
{
    return new ICmp(&this->ctx, op1, op2, conditionType);
}

void SymManager::branch(ICmp* condition, bool concreteCondition, void* trueLabel, void* falseLabel)
{
    z3::expr expr = condition->createExpr();
    if (!concreteCondition)
    {
        expr = !expr;
    }

    this->pathCondition.addCondition(expr);
    this->pathCondition.dump();
}