#pragma once

class RuntimeContext;

class InstrumentBlock
{
public:
    InstrumentBlock(RuntimeContext* context);
    ~InstrumentBlock();

    InstrumentBlock(const InstrumentBlock& other) = delete;
    InstrumentBlock operator=(const InstrumentBlock& other) = delete;

private:
    RuntimeContext* context;
};

using IBlock = InstrumentBlock;
