#include "MmapAllocator.h"
#include "../util/Logger.h"
#include "../Common.h"

#include <sys/mman.h>
#include <cassert>
#include <cstring>

void MmapAllocator::init(size_t size)
{
    if (this->base != nullptr) return;

    this->size = size;

    LOCK_MALLOC();

    void* address = ::mmap(nullptr, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);
    assert(address != MAP_FAILED);

    this->base = static_cast<char*>(address);
    this->memory = this->base;

    Logger::log("Mmap area: %p, size %lu\n", this->base, size);

    UNLOCK_MALLOC();
}

void* MmapAllocator::getBaseAddress() const
{
    return this->base;
}

void* MmapAllocator::alloc(size_t size)
{
    assert(size > 0);

    LOCK_MALLOC();

    char* address = this->align(this->memory);

#ifdef LOG_ALLOC
    Logger::log("MmapAllocator::alloc of %d, memory: %p\n", size, address);
#endif

    Logger::ensure((reinterpret_cast<size_t>(address) & 15) == 0, "memory is 16-byte aligned");

    // store size of block
    size_t* mem = reinterpret_cast<size_t*>(address);
    *mem = size;

    this->memory = (address + size + sizeof(size_t) + 32);  // 32 == boundary

    address += sizeof(size_t);
    std::memset(address, 0, size);

    UNLOCK_MALLOC();
    return address;
}

void* MmapAllocator::realloc(void* oldAddress, size_t size)
{
    LOCK_MALLOC();

    size_t oldSize = this->getBlockSize(oldAddress);
    assert(oldSize > 0);

#ifdef LOG_ALLOC
    Logger::log("Reallocating %lu bytes\n", oldSize);
#endif

    void* mem = this->alloc(size);
    std::memcpy(mem, oldAddress, oldSize);

    UNLOCK_MALLOC();
    return mem;
}

bool MmapAllocator::ownsMemory(void* address)
{
    return this->base <= address && address <= (this->base + this->size);
}
