#include "LinkerManager.h"

#include <dlfcn.h>
#include <cassert>


void LinkerManager::load_symbols()
{
    if (this->symbolsLoaded || this->loadInProgress)
    {
        return;
    }

    std::lock_guard<decltype(this->memoryMutex)> lock(this->memoryMutex);

    if (this->symbolsLoaded || this->loadInProgress)
    {
        return;
    }

    this->loadInProgress = true;

    if (!this->malloc_orig) this->load_symbol(this->malloc_orig, "malloc");
    if (!this->calloc_orig) this->load_symbol(this->calloc_orig, "calloc");
    if (!this->realloc_orig) this->load_symbol(this->realloc_orig, "realloc");
    if (!this->free_orig) this->load_symbol(this->free_orig, "free");

    this->symbolsLoaded = true;
    this->loadInProgress = false;
}

template<typename T>
T LinkerManager::load_symbol(T& target, const char* name)
{
    if (!target)
    {
        target = (T) dlsym(RTLD_NEXT, name);
        assert(target);
    }

    return target;
}

bool LinkerManager::isLoadingSymbols() const
{
    return this->loadInProgress;
}
