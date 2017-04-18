#pragma once

#include <stack>
#include <vector>

#include "../symbolic/Constraint.h"
#include "Frame.h"

class FrameManager
{
public:
    void push(std::vector<Constraint*> arguments);
    void pop();

    Constraint* getReturnValue() const;
    void setReturn(Constraint* constraint);

    const Frame& getCurrentFrame() const;

private:
    Constraint* returnValue = nullptr;
    std::stack<Frame> frames;
};
