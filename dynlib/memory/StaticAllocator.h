#pragma once

#include <cstdlib>
#include <cassert>
#include "../util/Logger.h"
#include "Allocator.h"

template <size_t SIZE = 1024>
class StaticAllocator : public Allocator
{
public:
    void* malloc(size_t size)
    {
        assert(this->index + size <= SIZE);
        size_t index = this->index;

        char* address = this->align(this->memory + index);
        this->index += size + (address - (this->memory + index));

        Logger::ensure((reinterpret_cast<size_t>(address) & 15) == 0, "memory is 16-byte aligned");

        return address;
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
