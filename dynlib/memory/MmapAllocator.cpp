#include "MmapAllocator.h"
#include "../util/Logger.h"

#include <sys/mman.h>
#include <cassert>
#include <cstring>

void MmapAllocator::init(size_t size)
{
    if (this->base != nullptr) return;

    void* address = ::mmap(nullptr, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);
    assert(address != MAP_FAILED);

    this->base = static_cast<char*>(address);
    this->memory = this->base;

    Logger::log("Mmap area: %p, size %lu\n", this->base, size);
}

void* MmapAllocator::getBaseAddress() const
{
    return this->base;
}

void* MmapAllocator::alloc(size_t size)
{
    char* address = this->align(this->memory);

#ifdef LOG_ALLOC
    Logger::log("MmapAllocator::alloc of %d, memory: %p\n", size, address);
#endif

    // store size of block
    size_t* mem = reinterpret_cast<size_t*>(address);
    *mem = size;

    this->memory = (address + size + sizeof(size_t) + 32);  // 32 == boundary

    Logger::ensure((reinterpret_cast<size_t>(address) & 7) == 0, "memory is 8-byte aligned");

    address += sizeof(size_t);
    std::memset(address, 0, size);
    return address;
}

void* MmapAllocator::realloc(void* oldAddress, size_t size)
{
    size_t oldSize = this->getBlockSize(oldAddress);
    void* mem = this->alloc(size);
    std::memcpy(mem, oldAddress, oldSize);
    return mem;
}
