#include "../util/Logger.h"
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

Constraint* SymManager::exprConst(size_t value, size_t size)
{
    return new Constant(&this->ctx, size, value);
}
Constraint* SymManager::exprLoad(void* address, size_t size)
{
    Constraint* constraint = this->getConstraint(address);
    if (constraint != nullptr) return constraint;

    return new Concrete(&this->ctx, size, address);
}
Constraint* SymManager::exprAdd(Constraint* op1, Constraint* op2, size_t size)
{
    return new Add(&this->ctx, size, op1, op2);
}
Constraint* SymManager::exprICmp(Constraint* op1, Constraint* op2, CmpType conditionType)
{
    return new ICmp(&this->ctx, op1, op2, conditionType);
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
void SymManager::branch(ICmp* condition, bool concreteCondition, void* trueLabel, void* falseLabel)
{
    z3::expr expr = condition->createExpr();
    if (!concreteCondition)
    {
        //expr = !expr;
    }

    //Logger::log("SAT: %d\n", this->pathCondition.isSatisfiable(expr));

    this->pathCondition.addCondition(expr);
}

void SymManager::checkGEP(MemoryManager* heapManager, void* address, Constraint* indexer, const char* location)
{
    Logger::log("Checking GEP (buffer at %p)\n", address);

    const MemoryBlock* block = heapManager->getBlock(address);
    if (block != nullptr)
    {
        Logger::log("GEP block found, block size %lu\n", block->getSize());
        indexer->dump(1);
        z3::expr indexerExpr = indexer->createExpr();
        z3::expr biggerIndex = indexerExpr >= this->ctx.int_val((unsigned long long) block->getSize());
        Logger::log("%s\n", Logger::stringify(biggerIndex));
        if (this->pathCondition.isSatisfiable(biggerIndex))
        {
            Logger::log("Access out of bounds possible at: %s\n", location);
        }
    }
    else Logger::log("GEP block not found\n");
}
