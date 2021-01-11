#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <cstdarg>
#include <stdint.h>
#include "NacosString.h"
#include "src/thread/Mutex.h"

#define DETAILED_DEBUG_INFO

//TODO:Line info
#ifndef DETAILED_DEBUG_INFO
#define log_print(level, format, args...) Logger::debug_print(level, format, ##args)
#define log_debug(format, args...) Logger::debug_debug(format, ##args)
#define log_info(format, args...) Logger::debug_info(format, ##args)
#define log_warn(format, args...) Logger::debug_warn(format, ##args)
#define log_error(format, args...) Logger::debug_error(format, ##args)
#else
#define STR(X) #X
#define log_print(level, format, args...) Logger::debug_print(level, format, ##args)
#define log_debug(format, args...) Logger::debug_debug(format, ##args)
#define log_info(format, args...) Logger::debug_info(format, ##args)
#define log_warn(format, args...) Logger::debug_warn(format, ##args)
#define log_error(format, args...) Logger::debug_error(format, ##args)
#endif

namespace nacos{

enum LOG_LEVEL {
    DEBUG = 0,
    INFO,
    WARN,
    ERROR,
    NONE
};

class Logger {
private:
    static LOG_LEVEL _CUR_SYS_LOG_LEVEL;
    static NacosString _log_base_dir;
    static NacosString _log_file;
    static int64_t _rotate_time;
    static int64_t _last_rotate_time;
    static FILE *_output_file;

    static Mutex setFileLock;

    static int debug_helper(LOG_LEVEL level, const char *format, va_list args);

    static void initializeLogSystem();

public:

    static void setRotateTime(int64_t rotateTime);
    static void setBaseDir(const NacosString &baseDir);
    static void setLogLevel(LOG_LEVEL level);

    static int64_t getRotateTime();
    static const NacosString &getBaseDir();
    static LOG_LEVEL getLogLevel();

    //Output string in self-defined log_level
    static int debug_print(LOG_LEVEL level, const char *format, ...);

    static int debug_debug(const char *format, ...);

    static int debug_info(const char *format, ...);

    static int debug_warn(const char *format, ...);

    static int debug_error(const char *format, ...);

    static void Init();
    static void deInit();
};
}//namespace nacos

#endif