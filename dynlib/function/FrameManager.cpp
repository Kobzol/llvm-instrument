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

}

void FrameManager::push()
{

}
