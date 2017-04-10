#include "Common.h"
#include "symbolic/Constant.h"
#include "../common/CmpType.h"

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

PUBLIC void CALLBACK(store)(void* memDst, size_t dstSize, void* memSrc)
{
    Constraint* constraint = static_cast<Constraint*>(memSrc);
    constraint->dump();
    runtimeContext->getSymManager()->store(memDst, dstSize, constraint);
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
    return runtimeContext->getSymManager()->exprConst(value, size);
}
PUBLIC void* CALLBACK(expr_load)(void* mem, size_t size)
{
    return runtimeContext->getSymManager()->exprLoad(mem, size);
}
PUBLIC void* CALLBACK(expr_add)(void* op1, void* op2, size_t size)
{
    return runtimeContext->getSymManager()->exprAdd(
            static_cast<Constraint*>(op1), static_cast<Constraint*>(op2), size);
}
PUBLIC void* CALLBACK(expr_icmp)(void* op1, void* op2, size_t type)
{
    return runtimeContext->getSymManager()->exprICmp(
            static_cast<Constraint*>(op1), static_cast<Constraint*>(op2), static_cast<CmpType>(type));
}


PUBLIC void CALLBACK(branch)(void* condition, void* trueLabel, void* falseLabel)
{
    Constraint* cond = static_cast<Constraint*>(condition);
    cond->dump();
}
