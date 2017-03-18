#pragma once

#include <cstdlib>
#include <string>

struct MapSegment
{
    MapSegment(size_t start, size_t length, bool readable, bool writable, bool executable, std::string path)
            : start(start), length(length), readable(readable), writable(writable), executable(executable), path(path)
    {

    }

    size_t start;
    size_t length;
    bool readable;
    bool writable;
    bool executable;
    std::string path;

    char* data = nullptr;
};
