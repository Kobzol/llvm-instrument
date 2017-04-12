#include "Formatter.h"

std::string Formatter::formatAddress(void* address)
{
    char buffer[128];
    snprintf(buffer, sizeof(buffer) - 1, "%p", address);

    return std::string(buffer);
}
