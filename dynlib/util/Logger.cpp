#include "Logger.h"

#include <unistd.h>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

char Logger::buffer[512];

void Logger::log(const char* msg, ...)
{
    va_list args;

    va_start(args, msg);
    vsnprintf(Logger::buffer, sizeof(Logger::buffer), msg, args);
    va_end(args);

    ::write(STDERR_FILENO, Logger::buffer, strlen(Logger::buffer));
}
void Logger::log(int level, const char* msg, ...)
{
    va_list args;

    memset(Logger::buffer, ' ', (size_t) level * 2);

    va_start(args, msg);
    vsnprintf(Logger::buffer + level * 2, sizeof(Logger::buffer) - level * 2, msg, args);
    va_end(args);

    ::write(STDERR_FILENO, Logger::buffer, strlen(Logger::buffer));
}

void Logger::ensure(bool condition, const char* msg)
{
    if (!condition)
    {
        Logger::log(msg);
        exit(1);
    }
}
