#pragma once

#include <cstddef>

class HeapBlock
{
public:
    HeapBlock()
    {

    }
    HeapBlock(void* address, size_t size) : address(address), size(size), active(true)
    {

    }

    void free()
    {
        this->active = false;
    }

    void* address;
    size_t size;
    bool active;
};
