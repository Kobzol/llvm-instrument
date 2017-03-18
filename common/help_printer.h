#pragma once

#include "stdarg.h"
#include "stdio.h"
#include "string.h"

#define LOG_INFO (0)
#define LOG_ERROR (1)
#define LOG_DEBUG (2)

class HelpPrinter {
public:
    static void print(int log_level, const char* string, ...);
};
