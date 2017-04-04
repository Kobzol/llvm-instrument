#include <memory>

#include "Common.h"

void* operator new(size_t size) throw (std::bad_alloc)
{
    void* mem = ::malloc(size);
    if (mem)
    {
        return mem;
    }
    else throw std::bad_alloc();
}

void operator delete(void* addr) throw()
{
    ::free(addr);
}

PUBLIC void* malloc(size_t size)
{
#ifdef LOG_ALLOC
    Logger::log("malloc %d\n", size);
#endif

    std::lock_guard<decltype(mutex)> lock(mutex);

    if (linkerManager.isLoadingSymbols())
    {
        return staticAllocator.malloc(size);
    }
    else linkerManager.load_symbols();

    initRuntime();

    if (runtimeContext->isInstrumenting())
    {
        return mmapAllocator.alloc(size);
    }

    void* addr = linkerManager.malloc_orig(size);
    {
        IBlock block(runtimeContext);
        runtimeContext->getHeapManager()->handle_malloc(addr, size);
    }
    return addr;
}

PUBLIC void* calloc(size_t count, size_t size)
{
#ifdef LOG_ALLOC
    Logger::log("calloc %d\n", count * size);
#endif

    std::lock_guard<decltype(mutex)> lock(mutex);

    if (linkerManager.isLoadingSymbols())
    {
        return staticAllocator.malloc(size);
    }
    else linkerManager.load_symbols();

    initRuntime();

    if (runtimeContext->isInstrumenting())
    {
        return mmapAllocator.alloc(count * size);
    }

    void* addr = linkerManager.calloc_orig(count, size);
    {
        IBlock block(runtimeContext);
        runtimeContext->getHeapManager()->handle_malloc(addr, count * size);
    }
    return addr;
}

PUBLIC void* realloc(void* addr, size_t size)
{
#ifdef LOG_ALLOC
    Logger::log("realloc %p %d\n", addr, size);
#endif

    std::lock_guard<decltype(mutex)> lock(mutex);

    if (linkerManager.isLoadingSymbols())
    {
        return staticAllocator.malloc(size);
    }
    else linkerManager.load_symbols();

    initRuntime();

    if (runtimeContext->isInstrumenting())
    {
        return mmapAllocator.realloc(addr, size);
    }

    void* newAddr = linkerManager.realloc_orig(addr, size);
    {
        IBlock block(runtimeContext);
        runtimeContext->getHeapManager()->handle_realloc(addr, newAddr, size);
    }
    return newAddr;
}

PUBLIC void free(void* addr)
{
#ifdef LOG_ALLOC
    Logger::log("free at %p\n", addr);
#endif

    std::lock_guard<decltype(mutex)> lock(mutex);

    if (linkerManager.isLoadingSymbols())
    {
        if (staticAllocator.owns_address(addr))
        {
            staticAllocator.free(addr);
        }
        return;
    }
    else linkerManager.load_symbols();

    initRuntime();

    if (runtimeContext->isInstrumenting())
    {
        return;
    }

    linkerManager.free_orig(addr);
    {
        IBlock block(runtimeContext);
        runtimeContext->getHeapManager()->handle_free(addr);
    }
}
