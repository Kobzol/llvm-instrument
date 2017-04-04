#include "HeapManager.h"
#include "../util/Logger.h"

void HeapManager::handle_malloc(void* address, size_t size)
{
    /*size_t hash = reinterpret_cast<size_t>(address);

    auto it = this->heapMap.find(hash);

    //Logger::ensure(it == this->heapMap.end() || !it->second.active, "block must be inactive");
    this->heapMap.insert({hash, HeapBlock(address, size)});*/
}

void HeapManager::handle_realloc(void* oldAddress, void* newAddress, size_t size)
{
    this->handle_free(oldAddress);
    this->handle_malloc(newAddress, size);
}

void HeapManager::handle_free(void* address)
{
    /*size_t hash = reinterpret_cast<size_t>(address);
    auto it = this->heapMap.find(hash);
    //Logger::ensure(it != this->heapMap.end() && it->second.active, "block muse be active");

    it->second.free();*/
}
