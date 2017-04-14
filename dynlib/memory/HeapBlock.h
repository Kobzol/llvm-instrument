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

    void* getAddress() const
    {
        return this->address;
    }
    bool isActive() const
    {
        return this->active;
    }
    size_t getSize() const
    {
        return this->size;
    }

private:
    void* address;
    size_t size;
    bool active;
};
