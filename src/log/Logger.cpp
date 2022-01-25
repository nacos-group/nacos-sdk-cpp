#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include "Logger.h"
#include "src/utils/TimeUtils.h"
#include "NacosExceptions.h"
#include "src/utils/ConfigParserUtils.h"
#include "Properties.h"
#include "src/utils/DirUtils.h"
#include "src/utils/ParamUtils.h"
#include "src/config/IOUtils.h"
#include "constant/ConfigConstant.h"
#include "constant/PropertyKeyConst.h"
#include <ctime>
#include <sys/stat.h>

namespace nacos{

LOG_LEVEL Logger::_CUR_SYS_LOG_LEVEL = ERROR;
NacosString Logger::_log_base_dir = "";
NacosString Logger::_log_file = "";
int64_t Logger::_rotate_size;
int64_t Logger::_last_rotate_time;
FILE *Logger::_output_file;
Mutex Logger::setFileLock;

//rotate time (in Ms)
void Logger::setRotateSize(int64_t rotateSize) {
    _rotate_size = rotateSize;
}

void Logger::setBaseDir(const NacosString &baseDir) {
    LockGuard _setFile(setFileLock);
    _log_base_dir = baseDir;
    if (_output_file != NULL) {
        fclose(_output_file);
        _output_file = NULL;
    }

    _log_file = _log_base_dir + ConfigConstant::FILE_SEPARATOR + "nacos-sdk-cpp.log";
    IOUtils::recursivelyCreate(_log_base_dir.c_str());
    _output_file = fopen(_log_file.c_str(), "a");
    if (_output_file == NULL) {
        NacosString errMsg = "Unable to open file ";
        errMsg += _log_file;
        throw NacosException(NacosException::UNABLE_TO_OPEN_FILE, errMsg);
    }
}

void Logger::setLogLevel(LOG_LEVEL level) {
    _CUR_SYS_LOG_LEVEL = level;
};

int64_t Logger::getRotateSize() {
    return _rotate_size;
}

const NacosString &Logger::getBaseDir() {
    return _log_base_dir;
}

LOG_LEVEL Logger::getLogLevel() {
    return _CUR_SYS_LOG_LEVEL;
}

int Logger::debug_helper(LOG_LEVEL level, const char *format, va_list args) {
    //Since the current system debug level is greater than this message
    //Supress it
    if (Logger::_CUR_SYS_LOG_LEVEL > level) {
        return 0;
    }
    //va_list argList;

    //va_start(argList, format);
    int64_t now = TimeUtils::getCurrentTimeInMs();

    struct stat stat_buf;
    stat(_log_file.c_str(), &stat_buf);
    if (stat_buf.st_size >= _rotate_size) {
        truncate(_log_file.c_str(), 0);
        _last_rotate_time = now;
    }

    const char *log_level;
    switch (level) {
        case DEBUG:
            log_level = "[DEBUG]";
            break;
        case INFO:
            log_level = "[INFO]";
            break;
        case WARN:
            log_level = "[WARN]";
            break;
        case ERROR:
            log_level = "[ERROR]";
            break;
        case NONE:
            log_level = "[NONE]";
            break;
        default:
            log_level = "[UNKNOWN]";
            break;
    }

    time_t t = time(0);
    struct tm current_time;
    localtime_r(&t, &current_time);
    //length of [9999-12-31 99:99:99] = 19
    char time_buf[22];
    strftime(time_buf, sizeof(time_buf), "[%Y-%m-%d %H:%M:%S]", &current_time);

    int retval = fprintf(_output_file, "%s%s", time_buf, log_level);
    retval += vfprintf(_output_file, format, args);
    fflush(_output_file);
    //va_end(argList);
    return retval;
}

//Output string in self-defined log_level
int Logger::debug_print(LOG_LEVEL level, const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    int retval = debug_helper(level, format, argList);
    va_end(argList);
    return retval;
}

int Logger::debug_debug(const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    int retval = debug_helper(DEBUG, format, argList);
    va_end(argList);
    return retval;
}

int Logger::debug_info(const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    int retval = debug_helper(INFO, format, argList);
    va_end(argList);
    return retval;
}

int Logger::debug_warn(const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    int retval = debug_helper(WARN, format, argList);
    va_end(argList);
    return retval;
}

int Logger::debug_error(const char *format, ...) {
    va_list argList;

    va_start(argList, format);
    int retval = debug_helper(ERROR, format, argList);
    va_end(argList);
    return retval;
}

void Logger::deInit() {
    if (_output_file != NULL) {
        fclose(_output_file);
        _output_file = NULL;
    }
}

void Logger::initializeLogSystem() {
    Properties props;

    try {
        props = ConfigParserUtils::parseConfigFile(DirUtils::getCwd() + ConfigConstant::FILE_SEPARATOR + ConfigConstant::DEFAULT_CONFIG_FILE);
    } catch (IOException &e) {
        //if we failed to read log settings
        //use default settings as backup
    }

    applyLogSettings(props);
}


void Logger::applyLogSettings(Properties &props) {
    if (!props.contains(PropertyKeyConst::LOG_PATH)) {
        NacosString homedir = DirUtils::getHome();
        Logger::setBaseDir(homedir + ConfigConstant::FILE_SEPARATOR + "nacos" + ConfigConstant::FILE_SEPARATOR + "logs");
    } else {
        Logger::setBaseDir(props[PropertyKeyConst::LOG_PATH]);
    }

    if (props.contains(PropertyKeyConst::LOG_LEVEL)) {
        //default log level is error, if user specifies it within configuration file, update it
        NacosString &logLevelStr = props[PropertyKeyConst::LOG_LEVEL];

        if (logLevelStr == "DEBUG") {
            Logger::setLogLevel(DEBUG);
        } else if (logLevelStr == "INFO") {
            Logger::setLogLevel(INFO);
        } else if (logLevelStr == "WARN") {
            Logger::setLogLevel(WARN);
        } else if (logLevelStr == "ERROR") {
            Logger::setLogLevel(ERROR);
        } else if (logLevelStr == "NONE") {
            Logger::setLogLevel(NONE);
        } else {
            throw NacosException(NacosException::INVALID_CONFIG_PARAM, "Invalid option " + logLevelStr + " for " + PropertyKeyConst::LOG_LEVEL);
        }
    }

    if (!props.contains(PropertyKeyConst::LOG_ROTATE_SIZE)) {
        Logger::setRotateSize(10 * 1024 *1024);//10M by default
    } else {
        const NacosString &logRotateSizeStr = props[PropertyKeyConst::LOG_ROTATE_SIZE];
        if (ParamUtils::isBlank(logRotateSizeStr)) {
            throw NacosException(NacosException::INVALID_CONFIG_PARAM,
                                 "Invalid option '" + logRotateSizeStr + "' for " + PropertyKeyConst::LOG_ROTATE_SIZE);
        }

        size_t logrotate_lastch = logRotateSizeStr.length() - 1;
        int mulplier = 1;
        unsigned long logRotateSize = 0;
        switch (logRotateSizeStr[logrotate_lastch])
        {
            case 'g':
            case 'G':
                mulplier *= 1024;
            case 'm':
            case 'M':
                mulplier *= 1024;
            case 'k':
            case 'K':
                mulplier *= 1024;
                logRotateSize = atol(logRotateSizeStr.substr(0, logrotate_lastch).c_str());//logrotate_lastch = exclude the unit
                logRotateSize *= mulplier;
                break;
            default:
                if (!isdigit(logRotateSizeStr[logrotate_lastch])) {
                    throw NacosException(NacosException::INVALID_CONFIG_PARAM,
                        "Invalid option '" + logRotateSizeStr + "' for " + PropertyKeyConst::LOG_ROTATE_SIZE + ", the unit of size must be G/g M/m K/k or decimal numbers.");

                }
                mulplier = 1;
                logRotateSize = atol(logRotateSizeStr.substr(0, logRotateSizeStr.length()).c_str());
                break;
        }

        if (logRotateSize <= 0) {
            throw NacosException(NacosException::INVALID_CONFIG_PARAM,
                                 PropertyKeyConst::LOG_ROTATE_SIZE + " should be greater than 0");
        }
        Logger::setRotateSize(logRotateSize);
    }

    log_info("Current log path:%s\n", Logger::getBaseDir().c_str());
}

void Logger::Init() {
    initializeLogSystem();
}

}//namespace nacos
