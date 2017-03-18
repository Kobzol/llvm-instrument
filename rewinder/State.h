#pragma once

#include <vector>
#include <sys/user.h>
#include "MapSegment.h"

class State
{
public:
    State(std::vector<MapSegment> segments, user_regs_struct regs, user_fpregs_struct fpregs)
            : segments(segments), regs(regs), fpregs(fpregs)
    {

    }

    const std::vector<MapSegment>& getSegments() const
    {
        return this->segments;
    }
    const user_regs_struct& getRegs() const
    {
        return regs;
    }
    const user_fpregs_struct& getFPRegs() const
    {
        return fpregs;
    }

private:
    std::vector<MapSegment> segments;
    user_regs_struct regs;
    user_fpregs_struct fpregs;
};
