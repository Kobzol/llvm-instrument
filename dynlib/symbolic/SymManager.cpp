#include "SymManager.h"
#include "../util/Logger.h"

void SymManager::makeSymbolic(void* mem, size_t size)
{
    size_t addr = reinterpret_cast<size_t>(mem);
    this->memoryConstraints.insert({addr, Constraint(&this->ctx, size)});
}

void SymManager::storeConst(void* memDst, size_t memSize, ssize_t constant, size_t constantSize)
{
    Constraint* constraint = this->getConstraint(memDst);
    if (constraint != nullptr)
    {
        Logger::ensure(memSize == constraint->getSize(), "sizes must match");

        constraint->setConstant(constant, constantSize);
    }
}

Constraint* SymManager::getConstraint(void* mem)
{
    size_t addr = reinterpret_cast<size_t>(mem);
    auto it = this->memoryConstraints.find(addr);
    if (it == this->memoryConstraints.end()) return nullptr;

    return &it->second;
}
