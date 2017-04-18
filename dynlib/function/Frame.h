#pragma once


#include <vector>
#include "../symbolic/Constraint.h"

class Frame
{
public:
    Frame(std::vector<Constraint*> arguments);

    Constraint* getParameter(size_t index) const;

private:
    std::vector<Constraint*> arguments;
};
