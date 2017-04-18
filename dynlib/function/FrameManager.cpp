#include <vector>
#include "FrameManager.h"

Constraint* FrameManager::getReturnValue() const
{
    return this->returnValue;
}
void FrameManager::setReturn(Constraint* constraint)
{
    this->returnValue = constraint;
}

void FrameManager::pop()
{
    if (this->frames.size() > 0)
    {
        this->frames.pop();
    }
}

void FrameManager::push(std::vector<Constraint*> arguments)
{
    this->frames.push(Frame(arguments));
}

const Frame& FrameManager::getCurrentFrame() const
{
    return this->frames.top();
}
