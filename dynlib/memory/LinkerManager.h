#pragma once

#include <cstdlib>
#include <atomic>
#include <mutex>

using malloc_orig_t = void* (*)(size_t size);
using calloc_orig_t = void* (*)(size_t count, size_t size);
using realloc_orig_t = void* (*)(void* addr, size_t size);
using free_orig_t = void (*)(void* addr);

class LinkerManager
{
public:
    malloc_orig_t malloc_orig = nullptr;
    calloc_orig_t calloc_orig = nullptr;
    realloc_orig_t realloc_orig = nullptr;
    free_orig_t free_orig = nullptr;

    void load_symbols();
    bool isLoadingSymbols() const;

private:
    template <typename T>
    T load_symbol(T& target, const char* name);

    std::atomic<bool> loadInProgress{false};
    std::recursive_mutex memoryMutex;

    bool symbolsLoaded = false;
};
