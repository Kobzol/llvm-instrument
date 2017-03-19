#pragma once

#include <unordered_map>
#include "../memory/HeapBlock.h"

class SymManager
{
public:
    void storeInt(void* memDst, size_t dstSize, void* memSrc, size_t srcSize);
    void storeIntConstant(void* memDst, size_t dstSize, ssize_t constant, size_t srcSize);

    void malloc(void* address, size_t size);
    void free(void* address);

    void init_fields()
    {
        if (this->heap == nullptr) this->heap = new std::unordered_map<size_t, HeapBlock>();
    }

    std::unordered_map<size_t, HeapBlock>* heap;

    void realloc(void* originalAddress, void* newAddress, size_t size);
};
