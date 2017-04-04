#include "RuntimeContext.h"
#include "../Common.h"

bool RuntimeContext::isInstrumenting() const
{
    return this->instrumentActive;
}

void RuntimeContext::startInstrumentation()
{
    this->instrumentActive = true;
}

void RuntimeContext::stopInstrumentation()
{
    this->instrumentActive = false;
}

void RuntimeContext::init()
{
    this->heapManager = new (mmapAllocator.alloc(sizeof(HeapManager))) HeapManager();
    this->symManager = new (mmapAllocator.alloc(sizeof(SymManager))) SymManager();
}

void RuntimeContext::connect(const char* host, unsigned short port)
{
    this->client.connect(host, port);
    assert(this->client.is_connected());
}

void RuntimeContext::setMmapArea(void* address, size_t size)
{
    this->client.writeline("set-mmap-area " + std::to_string((size_t) address) + " " + std::to_string(size));
}

HeapManager* RuntimeContext::getHeapManager() const
{
    return this->heapManager;
}

SymManager* RuntimeContext::getSymManager() const
{
    return this->symManager;
}
