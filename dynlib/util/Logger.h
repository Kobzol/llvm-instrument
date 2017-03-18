#pragma once

class Logger
{
public:
    static void log(const char* msg, ...);
    static void ensure(bool condition, const char* msg);

private:
    static char buffer[512];
};
