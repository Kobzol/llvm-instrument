#include "MemoryManager.h"
#include "../util/Logger.h"

void MemoryManager::handleMalloc(void* address, size_t size)
{
    this->insertBlock(this->heapMap, address, size);
}
void MemoryManager::handleRealloc(void* oldAddress, void* newAddress, size_t size)
{
    this->handleFree(oldAddress);
    this->handleMalloc(newAddress, size);
}
void MemoryManager::handleFree(void* address)
{
    auto it = this->heapMap.find(this->hashAddress(address));
    if (it != this->heapMap.end())
    {
        it->second.free();
    }
}

MemoryBlock* MemoryManager::getBlock(void* address)
{
    MemoryBlock* heapBlock = this->getBlock(address, this->heapMap);
    if (!heapBlock)
    {
        return this->getBlock(address, this->stackMap);
    }
    else return heapBlock;
}
MemoryBlock* MemoryManager::getBlock(void* address, MemoryMap& memoryMap)
{
    auto it = memoryMap.find(this->hashAddress(address));
    if (it == memoryMap.end())
    {
        return nullptr;
    }

    return &it->second;
}

size_t MemoryManager::hashAddress(void* address) const
{
    return reinterpret_cast<size_t>(address);
}

void MemoryManager::handleStackAlloc(void* address, size_t size)
{
    this->insertBlock(this->stackMap, address, size);
}

void MemoryManager::insertBlock(MemoryMap& memoryMap, void* address, size_t size)
{
    size_t hash = hashAddress(address);
    memoryMap[hash] = MemoryBlock(address, size);
}

void MemoryManager::handleStackDealloc(void* address)
{
    this->stackMap.erase(this->hashAddress(address));
}
