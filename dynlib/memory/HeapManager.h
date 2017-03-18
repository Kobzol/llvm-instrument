#pragma once

#include <cstdlib>
#include <unordered_map>

#include "HeapBlock.h"

class HeapManager
{
public:
    void handle_malloc(void* address, size_t size);
    void handle_realloc(void* oldAddress, void* newAddress, size_t size);
    void handle_free(void* address);

private:
    std::unordered_map<size_t, HeapBlock> heapMap;
};
