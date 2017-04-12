#include "Common.h"

StaticAllocator<STATIC_ALLOC_SIZE> staticAllocator;
MmapAllocator mmapAllocator;
LinkerManager linkerManager;
RuntimeContext* runtimeContext;
std::recursive_mutex mutex;
bool runtimeInitialized = false;
bool mallocInProgress = false;

void initRuntime()
{
    if (runtimeInitialized) return;
    runtimeInitialized = true;

    mmapAllocator.init(DYNAMIC_ALLOC_SIZE);
    void* mem = mmapAllocator.alloc(sizeof(RuntimeContext));

    runtimeContext = new (mem) RuntimeContext();
    {
        IBlock block(runtimeContext);
        runtimeContext->init();
    }
}
