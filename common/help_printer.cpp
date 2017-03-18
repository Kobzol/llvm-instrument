#include "help_printer.h"

#include <unistd.h>

void HelpPrinter::print(int log_level, const char* string, ...)
{
	va_list arg_list;

	char buffer[1024];

	va_start(arg_list, string);
	vsnprintf(buffer, sizeof(buffer), string, arg_list);
	va_end(arg_list);

	::write(STDERR_FILENO, buffer, strlen(buffer));
}
