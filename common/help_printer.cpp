#include "help_printer.h"

bool HelpPrinter::debug_mode = false;

void HelpPrinter::print(int log_level, const char* string, ...)
{
	if (log_level == LOG_DEBUG && !HelpPrinter::debug_mode) return;
		
	va_list arg_list;	
	
	if (log_level == LOG_ERROR)
	{
		char buffer[1024];
			
		va_start(arg_list, string);
		vsnprintf(buffer, sizeof(buffer), string, arg_list);
		va_end(arg_list);
		
		printf("ERROR: %s", buffer);
	}
	else
	{
		va_start(arg_list, string);
		vprintf(string, arg_list);
		va_end(arg_list); 
	}
}
