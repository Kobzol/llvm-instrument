#include "Common.h"

PUBLIC void CALLBACK(init)()
{
    Logger::log("SE init\n");

    initRuntime();

    IBlock block(runtimeContext);

    const char* hostEnv = "127.0.0.1";
    const char* portEnv = getenv("SE_PORT");
    if (hostEnv != nullptr && portEnv != nullptr)
    {
        Logger::log("Connecting to %s:%s\n", hostEnv, portEnv);
        runtimeContext->connect(hostEnv, (unsigned short) std::stoi(portEnv, nullptr, 10));
        runtimeContext->setMmapArea(mmapAllocator.getBaseAddress(), DYNAMIC_ALLOC_SIZE);
    }
}

PUBLIC void CALLBACK(store_const)(void* memDst, size_t dstSize, ssize_t constant, size_t srcSize)
{
    runtimeContext->getSymManager()->storeConst(memDst, dstSize, constant, srcSize);
}
PUBLIC void CALLBACK(store_mem)(void* memDst, size_t dstSize, void* memSrc, size_t srcSize)
{

}

PUBLIC void CALLBACK(make_symbolic)(void* mem, size_t size)
{
    runtimeContext->getSymManager()->makeSymbolic(mem, size);
}
