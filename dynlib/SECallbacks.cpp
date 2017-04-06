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

PUBLIC void CALLBACK(store)(void* memDst, size_t dstSize, void* memSrc, size_t srcSize)
{

}

PUBLIC void CALLBACK(make_symbolic)(void* mem, size_t size)
{
    runtimeContext->getSymManager()->makeSymbolic(mem, size);
}

PUBLIC void CALLBACK(print_addr)(void* mem)
{
    Logger::log("addr: %p\n", mem);
}

PUBLIC void* CALLBACK(expr_const)(size_t value, size_t size)
{
    Logger::log("expr_const: %lu (%lu bits)\n", value, size);

    return nullptr;
}

PUBLIC void* CALLBACK(expr_add)(__SExpression* value, size_t size)
{
    //Logger::log("expr_const: %lu (%lu bits)\n", value, size);

    return nullptr;
}
