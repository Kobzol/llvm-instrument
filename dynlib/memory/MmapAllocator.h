#pragma once

#include <cstdlib>

#include "Allocator.h"

class MmapAllocator : public Allocator
{
public:
    void init(size_t size);

    void* getBaseAddress() const;
    void* alloc(size_t address);
    void* realloc(void* oldAddress, size_t size);

private:
    char* base = nullptr;
    char* memory = nullptr;

    size_t getBlockSize(void* addr)
    {
        size_t* mem = reinterpret_cast<size_t*>(addr);
        return *(mem - 1);
    }
};
