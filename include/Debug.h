#ifndef __DEBUG_H_
#define __DEBUG_H_
#include <cstdarg>

#define log_print Debug::debug_print
#define log_debug Debug::debug_debug
#define log_info Debug::debug_info
#define log_warn Debug::debug_warn
#define log_error Debug::debug_error

enum LOG_LEVEL
{
	DEBUG = 0,
	INFO,
	WARN,
	ERROR,
	NONE
};

class Debug
{
private:
	static LOG_LEVEL _CUR_SYS_DEBUG_LEVEL;
	static int debug_helper(LOG_LEVEL level, const char *format, va_list args);
public:
	static int debug_print(LOG_LEVEL level, const char *format, ...);
	static int debug_debug(const char *format, ...);
	static int debug_info(const char *format, ...);
	static int debug_warn(const char *format, ...);
	static int debug_error(const char *format, ...);
	static void set_debug_level(LOG_LEVEL level) { _CUR_SYS_DEBUG_LEVEL = level; };
};

#endif