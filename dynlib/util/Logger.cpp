#include "Logger.h"

#include <unistd.h>
#include <cstring>
#include <cstdarg>
#include <cstdio>

char Logger::buffer[512];

void Logger::log(const char* msg, ...)
{
    va_list args;

    va_start(args, msg);
    vsprintf(Logger::buffer, msg, args);
    va_end(args);

    ::write(STDERR_FILENO, Logger::buffer, strlen(Logger::buffer));
}
