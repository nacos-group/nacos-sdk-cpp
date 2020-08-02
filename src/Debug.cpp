#include <stdio.h>
#include "Debug.h"

//The current debugging level for the system
LOG_LEVEL Debug::_CUR_SYS_DEBUG_LEVEL = ERROR;

int Debug::debug_helper(LOG_LEVEL level, const char *format, va_list args)
{
	//Since the current system debug level is greater than this message
	//Supress it
	if (Debug::_CUR_SYS_DEBUG_LEVEL > level)
	{
		return 0;
	}
	//va_list argList;

	//va_start(argList, format);
	int retval = vfprintf(stdout, format, args);
	//va_end(argList);
	return retval;
}

int Debug::debug_print(LOG_LEVEL level, const char *format, ...)
{
	va_list argList;

	va_start(argList, format);
	int retval = debug_helper(DEBUG, format, argList);
	va_end(argList);
	return retval;
}

int Debug::debug_debug(const char *format, ...)
{
	va_list argList;

	va_start(argList, format);
	int retval = debug_helper(DEBUG, format, argList);
	va_end(argList);
	return retval;
}

int Debug::debug_info(const char *format, ...)
{
	va_list argList;

	va_start(argList, format);
	int retval = debug_helper(INFO, format, argList);
	va_end(argList);
	return retval;
}

int Debug::debug_warn(const char *format, ...)
{
	va_list argList;

	va_start(argList, format);
	int retval = debug_helper(WARN, format, argList);
	va_end(argList);
	return retval;
}

int Debug::debug_error(const char *format, ...)
{
	va_list argList;

	va_start(argList, format);
	int retval = debug_helper(ERROR, format, argList);
	va_end(argList);
	return retval;
}