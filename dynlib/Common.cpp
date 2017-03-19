#include "Common.h"

StaticAllocator<STATIC_ALLOC_SIZE> staticAllocator;
MmapAllocator mmapAllocator;
LinkerManager linkerManager;
RuntimeContext* runtimeContext;
std::recursive_mutex mutex;

void initRuntime()
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    mmapAllocator.init(DYNAMIC_ALLOC_SIZE);
    runtimeContext = new (mmapAllocator.alloc(sizeof(RuntimeContext))) RuntimeContext();
    {
        IBlock block(runtimeContext);
        runtimeContext->init();
    }
}
