#include "InstrumentBlock.h"

#include "RuntimeContext.h"

InstrumentBlock::InstrumentBlock(RuntimeContext* context): context(context)
{
    context->startInstrumentation();
}

InstrumentBlock::~InstrumentBlock()
{
    context->stopInstrumentation();
}
