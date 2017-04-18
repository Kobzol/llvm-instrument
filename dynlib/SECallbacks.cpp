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
    if (!memSrc) return;

    IBlock block(runtimeContext);
    Constraint* constraint = static_cast<Constraint*>(memSrc);
    runtimeContext->getSymManager()->store(memDst, dstSize, constraint);
}

PUBLIC void CALLBACK(make_symbolic)(void* mem, size_t size)
{
    IBlock block(runtimeContext);
    runtimeContext->getSymManager()->makeSymbolic(mem, size);
}

PUBLIC void CALLBACK(print_addr)(void* mem)
{
    IBlock block(runtimeContext);
    Logger::log("addr: %p\n", mem);
}

PUBLIC void* CALLBACK(expr_const)(size_t value, size_t size)
{
    IBlock block(runtimeContext);
    return runtimeContext->getSymManager()->exprConst(value, size);
}
PUBLIC void* CALLBACK(expr_load)(void* mem, size_t size)
{
    IBlock block(runtimeContext);
    return runtimeContext->getSymManager()->exprLoad(mem, size);
}
PUBLIC void* CALLBACK(expr_add)(void* op1, void* op2, size_t size)
{
    IBlock block(runtimeContext);
    return runtimeContext->getSymManager()->exprAdd(
            static_cast<Constraint*>(op1), static_cast<Constraint*>(op2), size);
}
PUBLIC void* CALLBACK(expr_icmp)(void* op1, void* op2, size_t type)
{
    IBlock block(runtimeContext);
    return runtimeContext->getSymManager()->exprICmp(
            static_cast<Constraint*>(op1), static_cast<Constraint*>(op2), static_cast<CmpType>(type));
}

PUBLIC bool CALLBACK(branch)(void* condition, bool concreteCondition, void* trueLabel, void* falseLabel)
{
    IBlock block(runtimeContext);

    if (condition != nullptr)
    {
        ICmp* cond = static_cast<ICmp*>(condition);
        runtimeContext->getSymManager()->branch(cond, concreteCondition, trueLabel, falseLabel);
    }

    return concreteCondition;
}

PUBLIC void CALLBACK(checkGEP)(void* address, void* index)
{
    IBlock block(runtimeContext);

    Constraint* indexer = static_cast<Constraint*>(index);
    runtimeContext->getSymManager()->checkGEP(runtimeContext->getMemoryManager(), address, indexer);
}

PUBLIC void CALLBACK(stackAlloc)(void* address, size_t size)
{
    IBlock block(runtimeContext);
    runtimeContext->getMemoryManager()->handleStackAlloc(address, size);
}
PUBLIC void CALLBACK(stackDealloc)(void* address)
{
    IBlock block(runtimeContext);
    runtimeContext->getMemoryManager()->handleStackDealloc(address);
}
PUBLIC void CALLBACK(globalVariable)(void* address, size_t size)
{
    IBlock block(runtimeContext);
    runtimeContext->getMemoryManager()->handleGlobalVariable(address, size);
}

PUBLIC void CALLBACK(setReturnValue)(void* returnValue)
{
    IBlock block(runtimeContext);
    if (returnValue != nullptr)
    {
        ((Constraint*)returnValue)->dump();
    }
    runtimeContext->getFrameManager()->setReturn(static_cast<Constraint*>(returnValue));
}
PUBLIC void* CALLBACK(getReturnValue)()
{
    IBlock block(runtimeContext);
    return runtimeContext->getFrameManager()->getReturnValue();
}
