#pragma once

#include <mutex>
#include "memory/StaticAllocator.h"
#include "memory/MmapAllocator.h"
#include "memory/LinkerManager.h"
#include "instrument/RuntimeContext.h"
#include "instrument/InstrumentBlock.h"


#define PUBLIC extern "C"
#define CALLBACK(method) __se_##method

#define STATIC_ALLOC_SIZE (4096)
#define DYNAMIC_ALLOC_SIZE (512 * 1024 * 1024)

extern StaticAllocator<STATIC_ALLOC_SIZE> staticAllocator;
extern MmapAllocator mmapAllocator;
extern LinkerManager linkerManager;
extern RuntimeContext* runtimeContext;
extern std::recursive_mutex mutex;

void initRuntime();
