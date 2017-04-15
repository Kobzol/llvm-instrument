#pragma once

#include <cstdlib>
#include <unordered_map>

#include "MemoryBlock.h"

class MemoryManager
{
    using MemoryMap = std::unordered_map<size_t, MemoryBlock>;

public:
    void handleMalloc(void* address, size_t size);
    void handleRealloc(void* oldAddress, void* newAddress, size_t size);
    void handleFree(void* address);

    MemoryBlock* getBlock(void* address);

    void handleStackAlloc(void* address, size_t size);

    void handleStackDealloc(void* address);

private:
    size_t hashAddress(void* address) const;

    void insertBlock(MemoryMap& memoryMap, void* address, size_t size);
    MemoryBlock* getBlock(void* address, MemoryMap& memoryMap);

    MemoryMap heapMap;
    MemoryMap stackMap;
};
