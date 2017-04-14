#include "HeapManager.h"
#include "../util/Logger.h"

void HeapManager::handleMalloc(void* address, size_t size)
{
    size_t hash = this->hashAddress(address);

    //Logger::ensure(it == this->heapMap.end() || !it->second.active, "block must be inactive");
    this->heapMap.insert({hash, HeapBlock(address, size)});
}
void HeapManager::handleRealloc(void* oldAddress, void* newAddress, size_t size)
{
    this->handleFree(oldAddress);
    this->handleMalloc(newAddress, size);
}
void HeapManager::handleFree(void* address)
{
    auto it = this->heapMap.find(this->hashAddress(address));
    //Logger::ensure(it != this->heapMap.end() && it->second.active, "block muse be active");

    if (it != this->heapMap.end())
    {
        it->second.free();
    }
}

HeapBlock* HeapManager::getBlock(void* address)
{
    auto it = this->heapMap.find(this->hashAddress(address));
    if (it == this->heapMap.end())
    {
        return nullptr;
    }

    return &it->second;
}

size_t HeapManager::hashAddress(void* address) const
{
    return reinterpret_cast<size_t>(address);
}
