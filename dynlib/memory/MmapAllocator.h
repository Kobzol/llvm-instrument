#pragma once

#include <cstdlib>

class MmapAllocator
{
public:
    void init(size_t size);

    void* getAddress() const;
    void* alloc(size_t address);

private:
    char* memory = nullptr;
    size_t index = 0;
};
