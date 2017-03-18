#include "MmapAllocator.h"
#include "../util/Logger.h"

#include <sys/mman.h>
#include <cassert>
#include <cstring>

void MmapAllocator::init(size_t size)
{
    if (this->memory != nullptr) return;

    void* address = ::mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
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
    char* address = this->align(this->memory + index);
    this->index += size + (address - (this->memory + index));

    Logger::ensure((reinterpret_cast<size_t>(address) & 15) == 0, "memory is 16-byte aligned");

    memset(address, 0, size);
    return address;
}
