#pragma once

#include <mutex>
#include "memory/StaticAllocator.h"
#include "memory/MmapAllocator.h"
#include "memory/LinkerManager.h"
#include "instrument/RuntimeContext.h"
#include "instrument/InstrumentBlock.h"


//#define LOG_ALLOC
#define REPLACE_MALLOC
#define PROVIDE_INSTRUMENTATION_MALLOC

#define LOCK_MALLOC() do { mallocInProgress = true; } while (0);
#define UNLOCK_MALLOC() do { mallocInProgress = false; } while (0);
#define CHECK_MALLOC_LOCK() do { assert(!mallocInProgress); } while (0);

#define PUBLIC extern "C"
#define CALLBACK(method) __se_##method

#define STATIC_ALLOC_SIZE (4096)
#define DYNAMIC_ALLOC_SIZE (512 * 1024 * 1024)

extern StaticAllocator<STATIC_ALLOC_SIZE> staticAllocator;
extern MmapAllocator mmapAllocator;
extern LinkerManager linkerManager;
extern RuntimeContext* runtimeContext;
extern std::recursive_mutex mutex;
extern bool runtimeInitialized;
extern bool mallocInProgress;

void initRuntime();
