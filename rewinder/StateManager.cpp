#include "StateManager.h"

#include "MapSegment.h"
#include "State.h"

#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <iostream>
#include <sys/uio.h>
#include <cassert>
#include <sys/user.h>
#include <sys/ptrace.h>


std::unique_ptr<State> StateManager::save(pid_t pid)
{
    std::vector<MapSegment> segments = this->readSegments(pid);
    this->loadSegments(segments, pid);

    user_regs_struct regs;
    user_fpregs_struct fpregs;

    ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
    ptrace(PTRACE_GETFPREGS, pid, nullptr, &fpregs);

    return std::make_unique<State>(segments, regs, fpregs);
}

void StateManager::restore(pid_t pid, const State* state)
{
    this->storeSegments(state->getSegments(), pid);

    ptrace(PTRACE_SETREGS, pid, nullptr, &state->getRegs());
    ptrace(PTRACE_SETFPREGS, pid, nullptr, &state->getFPRegs());
}

std::vector<MapSegment> StateManager::readSegments(pid_t pid)
{
    std::vector<MapSegment> segments;
    std::ifstream fs("/proc/" + std::to_string(pid) + "/maps");
    std::string line;
    std::smatch match;
    std::regex regex("([a-fA-F0-9]+)-([a-fA-F0-9]+) ([r-])([w-])([x-]).*");

    if (!fs.is_open())
    {
        throw "Couldn't not open " + std::to_string(pid);
    }

    while (std::getline(fs, line))
    {
        if (std::regex_match(line, match, regex))
        {
            std::string start = match[1];
            std::string end = match[2];
            std::string readable = match[3];
            std::string writable = match[4];
            std::string executable = match[5];
            std::string path = "";

            size_t it = line.find_first_of("/");
            if (it != std::string::npos)
            {
                path = line.substr(it);
            }

            size_t pos;
            size_t base = std::stoul(start, &pos, 16);
            size_t baseEnd = std::stoul(end, &pos, 16);
            segments.emplace_back(base, baseEnd - base, readable == "r", writable == "w", executable == "x", path);
        }
        else std::cerr << "Couldn't parse map line " + line << std::endl;
    }

    return segments;
}
void StateManager::loadSegments(std::vector<MapSegment>& segments, pid_t pid)
{
    std::ifstream fs("/proc/" + std::to_string(pid) + "/mem");
    for (auto& segment : segments)
    {
        if (this->shouldReadSegment(segment))
        {
            fs.seekg(segment.start);
            if (!fs) throw "error reading";
            segment.data = (char*) malloc(segment.length);
            fs.read(segment.data, segment.length);
        }
    }
}
/*void StateManager::storeSegments(const std::vector<MapSegment>& segments, pid_t pid)
{
    std::string path = "/proc/" + std::to_string(pid) + "/mem";
    FILE* file = fopen(path.c_str(), "w");
    setvbuf(file, nullptr, _IONBF, 0);

    for (auto& segment : segments)
    {
        if (segment.data != nullptr)
        {
            fseek(file, segment.start, SEEK_SET);
            fwrite(segment.data, 1, segment.length, file);
            fflush(file);
        }
    }

    fclose(file);
}*/
void StateManager::storeSegments(const std::vector<MapSegment>& segments, pid_t pid)
{
    for (auto& segment : segments)
    {
        if (this->shouldWriteSegment(segment))
        {
            iovec targetAddr;
            targetAddr.iov_base = (void*) segment.start;
            targetAddr.iov_len = segment.length;
            iovec localAddr;
            localAddr.iov_base = segment.data;
            localAddr.iov_len = segment.length;

            ssize_t written = process_vm_writev(pid, &localAddr, 1, &targetAddr, 1, 0);
            assert((size_t) written == segment.length);
        }
    }
}

bool StateManager::shouldReadSegment(const MapSegment& segment) const
{
    return
            segment.readable &&
            segment.writable &&
            segment.path.find("libz3.so") == std::string::npos &&
                    (segment.start < this->guardedStart ||
                    segment.start + segment.length >= this->guardedStart + this->guardedLength);
}

bool StateManager::shouldWriteSegment(const MapSegment& segment) const
{
    return segment.data != nullptr;
}

void StateManager::setGuardedArea(size_t start, size_t length)
{
    this->guardedStart = start;
    this->guardedLength = length;
}
