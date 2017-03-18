#pragma once

#include <cstdlib>
#include <cassert>

template <size_t SIZE = 1024>
class StaticAllocator
{
public:
    void* malloc(size_t size)
    {
        assert(this->index + size <= SIZE);
        size_t index = this->index;
        this->index += size;
        return this->memory + index;
    }

    void free(void* address)
    {

    }

    bool owns_address(void* address)
    {
        return address >= this->memory && address < this->memory + SIZE;
    }

private:
    size_t index = 0;
    char memory[SIZE] = { 0 };
};
