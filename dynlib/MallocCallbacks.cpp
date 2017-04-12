#include <memory>

#include "Common.h"

#ifdef REPLACE_MALLOC
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
    CHECK_MALLOC_LOCK();

#ifdef LOG_ALLOC
    Logger::log("malloc %d\n", size);
#endif

    std::lock_guard<decltype(mutex)> lock(mutex);

    if (linkerManager.isLoadingSymbols())
    {
        return staticAllocator.malloc(size);
    }
    else linkerManager.load_symbols();

#ifdef PROVIDE_INSTRUMENTATION_MALLOC
    if (runtimeInitialized && runtimeContext->isInstrumenting())
    {
        return mmapAllocator.alloc(size);
    }
#endif

    void* addr = linkerManager.malloc_orig(size);
#ifdef PROVIDE_INSTRUMENTATION_MALLOC
    if (runtimeInitialized)
    {
        IBlock block(runtimeContext);
        runtimeContext->getHeapManager()->handle_malloc(addr, size);
    }
#endif
    return addr;
}

PUBLIC void* calloc(size_t count, size_t size)
{
    CHECK_MALLOC_LOCK();

#ifdef LOG_ALLOC
    Logger::log("calloc %d\n", count * size);
#endif

    std::lock_guard<decltype(mutex)> lock(mutex);

    if (linkerManager.isLoadingSymbols())
    {
        return staticAllocator.malloc(size);
    }
    else linkerManager.load_symbols();

#ifdef PROVIDE_INSTRUMENTATION_MALLOC
    if (runtimeInitialized && runtimeContext->isInstrumenting())
    {
        return mmapAllocator.alloc(count * size);
    }
#endif

    void* addr = linkerManager.calloc_orig(count, size);
#ifdef PROVIDE_INSTRUMENTATION_MALLOC
    if (runtimeInitialized)
    {
        IBlock block(runtimeContext);
        runtimeContext->getHeapManager()->handle_malloc(addr, count * size);
    }
#endif
    return addr;
}

PUBLIC void* realloc(void* addr, size_t size)
{
    CHECK_MALLOC_LOCK();

#ifdef LOG_ALLOC
    Logger::log("realloc %p %d\n", addr, size);
#endif

    std::lock_guard<decltype(mutex)> lock(mutex);

    if (linkerManager.isLoadingSymbols())
    {
        return staticAllocator.malloc(size);
    }
    else linkerManager.load_symbols();

#ifdef PROVIDE_INSTRUMENTATION_MALLOC
    if (runtimeInitialized && runtimeContext->isInstrumenting())
    {
        return mmapAllocator.realloc(addr, size);
    }
#endif

    void* newAddr = linkerManager.realloc_orig(addr, size);
#ifdef PROVIDE_INSTRUMENTATION_MALLOC
    if (runtimeInitialized)
    {
        IBlock block(runtimeContext);
        runtimeContext->getHeapManager()->handle_realloc(addr, newAddr, size);
    }
#endif
    return newAddr;
}

PUBLIC void free(void* addr)
{
    CHECK_MALLOC_LOCK();

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

#ifdef PROVIDE_INSTRUMENTATION_MALLOC
    if (runtimeInitialized && (runtimeContext->isInstrumenting()  ||
            mmapAllocator.ownsMemory(addr)))
    {
        return;
    }
#endif

    linkerManager.free_orig(addr);
#ifdef PROVIDE_INSTRUMENTATION_MALLOC
    if (runtimeInitialized)
    {
        IBlock block(runtimeContext);
        runtimeContext->getHeapManager()->handle_free(addr);
    }
#endif
}
#endif
