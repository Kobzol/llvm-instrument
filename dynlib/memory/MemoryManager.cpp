#include "MemoryManager.h"
#include "../util/Logger.h"

void MemoryManager::handleMalloc(void* address, size_t size)
{
    this->insertBlock(address, size, MemoryBlock::MemoryRegion::Heap);
}
void MemoryManager::handleRealloc(void* oldAddress, void* newAddress, size_t size)
{
    this->handleFree(oldAddress);
    this->handleMalloc(newAddress, size);
}
void MemoryManager::handleFree(void* address)
{
    auto it = this->memoryMap.find(this->hashAddress(address));
    if (it != this->memoryMap.end())
    {
        it->second.free();
    }
}

MemoryBlock* MemoryManager::getBlock(void* address)
{
    auto it = this->memoryMap.find(this->hashAddress(address));
    if (it == this->memoryMap.end())
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
    this->insertBlock(address, size, MemoryBlock::MemoryRegion::Stack);
}

void MemoryManager::insertBlock(void* address, size_t size, MemoryBlock::MemoryRegion region)
{
    size_t hash = hashAddress(address);
    memoryMap[hash] = MemoryBlock(address, size, region);
}

void MemoryManager::handleStackDealloc(void* address)
{
    this->memoryMap.erase(this->hashAddress(address));
}

void MemoryManager::handleGlobalVariable(void* address, size_t size)
{
    this->insertBlock(address, size, MemoryBlock::MemoryRegion::Data);
}
