#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <dlfcn.h>
#include <unistd.h>
#include <mutex>
#include <memory>
#include <iostream>

#include "memory/LinkerManager.h"
#include "memory/StaticAllocator.h"
#include "instrument/RuntimeContext.h"
#include "memory/MmapAllocator.h"
#include "util/Logger.h"
#include "instrument/InstrumentBlock.h"

#define STATIC_ALLOC_SIZE (4096)
#define DYNAMIC_ALLOC_SIZE (512 * 1024 * 1024)

static StaticAllocator<STATIC_ALLOC_SIZE> staticAllocator;
static MmapAllocator mmapAllocator;
static LinkerManager linkerManager;
static RuntimeContext* runtimeContext;
static std::recursive_mutex mutex;

static void init()
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    Logger::log("init inside\n");

    mmapAllocator.init(DYNAMIC_ALLOC_SIZE);
    runtimeContext = new (mmapAllocator.alloc(sizeof(RuntimeContext))) RuntimeContext();
    {
        IBlock block(runtimeContext);
        runtimeContext->init();
    }
}

extern "C"
{
    void* malloc(size_t size)
    {
        std::lock_guard<decltype(mutex)> lock(mutex);

        if (linkerManager.isLoadingSymbols())
        {
            return staticAllocator.malloc(size);
        }
        else linkerManager.load_symbols();

        init();

        if (runtimeContext->isInstrumenting())
        {
            return mmapAllocator.alloc(size);
        }

        void* addr = linkerManager.malloc_orig(size);
        return addr;
    }
    void* calloc(size_t count, size_t size)
    {
        std::lock_guard<decltype(mutex)> lock(mutex);

        if (linkerManager.isLoadingSymbols())
        {
            return staticAllocator.malloc(size);
        }
        else linkerManager.load_symbols();

        init();

        if (runtimeContext->isInstrumenting())
        {
            return mmapAllocator.alloc(count * size);
        }

        void* addr = linkerManager.calloc_orig(count, size);
        return addr;
    }
    void* realloc(void* addr, size_t size)
    {
        std::lock_guard<decltype(mutex)> lock(mutex);

        if (linkerManager.isLoadingSymbols())
        {
            return staticAllocator.malloc(size);
        }
        else linkerManager.load_symbols();

        init();

        if (runtimeContext->isInstrumenting())
        {
            assert(false);  // TODO: handle realloc properly
            return mmapAllocator.alloc(size);
        }

        void* newAddr = linkerManager.realloc_orig(addr, size);
        return newAddr;
    }
    void free(void* addr)
    {
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

        init();

        if (runtimeContext->isInstrumenting())
        {

        }

        linkerManager.free_orig(addr);
    }

    void __se_init()
    {
        Logger::log("SE init\n");

        init();

        const char* portEnv = getenv("SE_PORT");
        if (portEnv != nullptr)
        {
            Logger::log("Connecting to %s\n", portEnv);
            runtimeContext->connect("127.0.0.1", (unsigned short) std::stoi(portEnv, nullptr, 10));
            runtimeContext->setMmapArea(mmapAllocator.getAddress(), DYNAMIC_ALLOC_SIZE);
        }
    }

    void __se_store_const(void* memDst, size_t dstSize, ssize_t constant, size_t srcSize)
    {

    }
    void __se_store_mem(void* memDst, size_t dstSize, void* memSrc, size_t srcSize)
    {

    }
}
