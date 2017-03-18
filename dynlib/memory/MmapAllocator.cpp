#include "MmapAllocator.h"
#include "../util/Logger.h"

#include <sys/mman.h>
#include <cassert>

void MmapAllocator::init(size_t size)
{
    if (this->memory != nullptr) return;

    void* address = ::mmap((void*) 0xFFFFFFF85, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    assert(address);

    this->memory = static_cast<char*>(address);
}

void* MmapAllocator::getAddress() const
{
    return this->memory;
}

void* MmapAllocator::alloc(size_t size)
{
    size_t index = this->index;
    this->index += size;

    return this->memory + index;
}
