#ifndef __DEBUG_H_
#define __DEBUG_H_

#include <cstdarg>

#define DETAILED_DEBUG_INFO

//TODO:Line info
#ifndef DETAILED_DEBUG_INFO
#define log_print(level, format, args...) Debug::debug_print(level, format, ##args)
#define log_debug(format, args...) Debug::debug_debug(format, ##args)
#define log_info(format, args...) Debug::debug_info(format, ##args)
#define log_warn(format, args...) Debug::debug_warn(format, ##args)
#define log_error(format, args...) Debug::debug_error(format, ##args)
#else
#define STR(X) #X
#define log_print(level, format, args...) Debug::debug_print(level, format, ##args)
#define log_debug(format, args...) Debug::debug_debug(format, ##args)
#define log_info(format, args...) Debug::debug_info(format, ##args)
#define log_warn(format, args...) Debug::debug_warn(format, ##args)
#define log_error(format, args...) Debug::debug_error(format, ##args)
#endif

namespace nacos{
enum LOG_LEVEL {
    DEBUG = 0,
    INFO,
    WARN,
    ERROR,
    NONE
};

class Debug {
private:
    static LOG_LEVEL _CUR_SYS_DEBUG_LEVEL;

    static int debug_helper(LOG_LEVEL level, const char *format, va_list args);

public:
    //Output string in self-defined log_level
    static int debug_print(LOG_LEVEL level, const char *format, ...);

    static int debug_debug(const char *format, ...);

    static int debug_info(const char *format, ...);

    static int debug_warn(const char *format, ...);

    static int debug_error(const char *format, ...);

    static void set_debug_level(LOG_LEVEL level) { _CUR_SYS_DEBUG_LEVEL = level; };
};
}//namespace nacos

#endif