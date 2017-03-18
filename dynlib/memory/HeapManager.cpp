#include "HeapManager.h"

#include <cassert>

void HeapManager::handle_malloc(void* address, size_t size)
{
    size_t hash = reinterpret_cast<size_t>(address);

    auto it = this->heapMap.find(hash);

    assert(it == this->heapMap.end() || !it->second.active);
    this->heapMap.insert({hash, HeapBlock(address, size)});
}

void HeapManager::handle_realloc(void* oldAddress, void* newAddress, size_t size)
{
    this->handle_free(oldAddress);
    this->handle_malloc(newAddress, size);
}

void HeapManager::handle_free(void* address)
{
    size_t hash = reinterpret_cast<size_t>(address);
    auto it = this->heapMap.find(hash);
    assert(it != this->heapMap.end() && it->second.active);

    it->second.free();
}
