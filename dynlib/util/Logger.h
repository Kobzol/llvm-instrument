#pragma once

#include <z3++.h>

class Logger
{
public:
    static void log(const char* msg, ...);
    static void log(int level, const char* msg, ...);
    static void ensure(bool condition, const char* msg);

    static const char* stringify(const z3::expr& expr);

private:
    static char buffer[512];
};
