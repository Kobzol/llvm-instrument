#pragma once

#include <cstddef>

class MemoryBlock
{
public:
    enum class MemoryRegion: unsigned int
    {
        Stack = 0,
        Heap,
        Data
    };

    MemoryBlock()
    {

    }
    MemoryBlock(void* address, size_t size, MemoryRegion region)
            : address(address), size(size), active(true), region(region)
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
    MemoryRegion getRegion() const
    {
        return this->region;
    }

private:
    void* address;
    size_t size;
    bool active;
    MemoryRegion region;
};
