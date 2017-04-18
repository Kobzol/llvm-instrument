#pragma once

#include "../../common/socket.h"
#include "../memory/MemoryManager.h"
#include "../symbolic/SymManager.h"
#include "../function/FrameManager.h"

class RuntimeContext
{
public:
    void init();
    void connect(const char* host, unsigned short port);

    bool isInstrumenting() const;
    void startInstrumentation();
    void stopInstrumentation();

    void setMmapArea(void* address, size_t size);

    MemoryManager* getMemoryManager() const;
    SymManager* getSymManager() const;
    FrameManager* getFrameManager() const;

private:
    bool instrumentActive = false;
    Socket client;

    MemoryManager* heapManager;
    SymManager* symManager;
    FrameManager* frameManager;
};
