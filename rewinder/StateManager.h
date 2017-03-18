#pragma once

#include <cstdlib>
#include <memory>
#include "State.h"

class StateManager
{
public:
    std::unique_ptr<State> save(pid_t pid);
    void restore(pid_t pid, const State* state);

    void setGuardedArea(size_t start, size_t length);

private:
    std::vector<MapSegment> readSegments(pid_t pid);
    void loadSegments(std::vector<MapSegment>& segments, pid_t pid);
    void storeSegments(const std::vector<MapSegment>& segments, pid_t pid);

    bool shouldReadSegment(const MapSegment& mapSegment) const;
    bool shouldWriteSegment(const MapSegment& mapSegment) const;

    size_t guardedStart;
    size_t guardedLength;
};
