#pragma once

#include <atomic>
#include "../../common/socket.h"
#include "../memory/HeapManager.h"

class RuntimeContext
{
public:
    void init();
    void connect(const char* host, unsigned short port);

    bool isInstrumenting() const;
    void startInstrumentation();
    void stopInstrumentation();

    void setMmapArea(void* address, size_t size);

    HeapManager* getHeapManager() const;

private:
    std::atomic<bool> instrumentActive{false};
    Socket client;

    HeapManager* heapManager;
};
