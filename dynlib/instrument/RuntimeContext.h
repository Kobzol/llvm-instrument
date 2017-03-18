#pragma once

#include <atomic>
#include "../../common/socket.h"

class RuntimeContext
{
public:
    void init();
    void connect(const char* host, unsigned short port);

    bool isInstrumenting() const;
    void startInstrumentation();
    void stopInstrumentation();

//private:
    std::atomic<bool> instrumentActive{false};
    Socket client;
    int* p;

    void setMmapArea(void* address, size_t size);
};
