#pragma once

#include "../../common/socket.h"
#include "../memory/HeapManager.h"
#include "../symbolic/SymManager.h"

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
    SymManager* getSymManager() const;

    bool ownsMemory(void* address);

private:
    bool instrumentActive = false;
    Socket client;

    HeapManager* heapManager;
    SymManager* symManager;
};
