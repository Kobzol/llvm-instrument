#include "RuntimeContext.h"

#include <cassert>
#include <iostream>

bool RuntimeContext::isInstrumenting() const
{
    return this->instrumentActive;
}

void RuntimeContext::startInstrumentation()
{
    this->instrumentActive.store(true);
}

void RuntimeContext::stopInstrumentation()
{
    this->instrumentActive.store(false);
}

void RuntimeContext::init()
{
    this->heapManager = new HeapManager();
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
    return heapManager;
}
