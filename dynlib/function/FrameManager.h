#pragma once

#include <stack>

#include "../symbolic/Constraint.h"
#include "Frame.h"

class FrameManager
{
public:
    void push();
    void pop();

    Constraint* getReturnValue() const;
    void setReturn(Constraint* constraint);
private:
    Constraint* returnValue = nullptr;
    std::stack<Frame> frames;
};
