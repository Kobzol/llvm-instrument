#include "Frame.h"

Frame::Frame(std::vector<Constraint*> arguments): arguments(arguments)
{

}

Constraint* Frame::getParameter(size_t index) const
{
    return this->arguments.at(index);
}
