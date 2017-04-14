#pragma once

#include <cstdlib>
#include <unordered_map>

#include "HeapBlock.h"

class HeapManager
{
public:
    void handleMalloc(void* address, size_t size);
    void handleRealloc(void* oldAddress, void* newAddress, size_t size);
    void handleFree(void* address);

    HeapBlock* getBlock(void* address);

private:
    size_t hashAddress(void* address) const;

    std::unordered_map<size_t, HeapBlock> heapMap;
};
