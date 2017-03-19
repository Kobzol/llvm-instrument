#include "SymManager.h"

void SymManager::storeInt(void* memDst, size_t dstSize, void* memSrc, size_t srcSize)
{

}

void SymManager::storeIntConstant(void* memDst, size_t dstSize, ssize_t constant, size_t srcSize)
{

}

void SymManager::malloc(void* address, size_t size)
{
    size_t hash = reinterpret_cast<size_t>(address);
    this->heap->insert({hash, HeapBlock(address, size)});
}

void SymManager::free(void* address)
{
    size_t hash = reinterpret_cast<size_t>(address);

    auto it = this->heap->find(hash);
    if (it != this->heap->end())
    {
        (*this->heap)[hash].free();
    }
}

void SymManager::realloc(void* originalAddress, void* newAddress, size_t size)
{
    this->free(originalAddress);
    this->malloc(newAddress, size);
}
