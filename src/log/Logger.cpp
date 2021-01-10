#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include "Logger.h"
#include "src/utils/TimeUtils.h"
#include "NacosExceptions.h"
#include "src/utils/ConfigParserUtils.h"
#include "Properties.h"
#include "utils/DirUtils.h"
#include "utils/ParamUtils.h"
#include "src/config/IOUtils.h"
#include "constant/ConfigConstant.h"
#include "constant/PropertyKeyConst.h"

namespace nacos{

LOG_LEVEL Logger::_CUR_SYS_LOG_LEVEL = ERROR;
NacosString Logger::_log_base_dir = "";
NacosString Logger::_log_file = "";
int64_t Logger::_rotate_time;
int64_t Logger::_last_rotate_time;
FILE *Logger::_output_file;
Mutex Logger::setFileLock;

//rotate time (in Ms)
void Logger::setRotateTime(int64_t rotateTime) {
    _rotate_time = rotateTime;
}

void Logger::setBaseDir(const NacosString &baseDir) {
    LockGuard _setFile(setFileLock);
    _log_base_dir = baseDir;
    if (_output_file != NULL) {
        fclose(_output_file);
        _output_file = NULL;
    }

    IOUtils::recursivelyCreate(_log_base_dir.c_str());
    _log_file = _log_base_dir + ConfigConstant::FILE_SEPARATOR + "nacos-sdk-cpp.log";
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

int64_t Logger::getRotateTime() {
    return _rotate_time;
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
    if (now - _last_rotate_time >= _rotate_time) {
        truncate(_log_file.c_str(), 0);
        _last_rotate_time = now;
    }

    int retval = vfprintf(_output_file, format, args);
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

    //if we failed to read log settings
    props = ConfigParserUtils::parseConfigFile(DirUtils::getCwd() + ConfigConstant::FILE_SEPARATOR + ConfigConstant::DEFAULT_CONFIG_FILE);

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

    if (!props.contains(PropertyKeyConst::LOG_ROTATE_TIME)) {
        Logger::setRotateTime(24 * 60 * 60 * 1000);
    } else {
        const NacosString &logRotateTimeStr = props[PropertyKeyConst::LOG_ROTATE_TIME];
        if (ParamUtils::isBlank(logRotateTimeStr)) {
            throw NacosException(NacosException::INVALID_CONFIG_PARAM,
                                 "Invalid option " + logRotateTimeStr + " for " + PropertyKeyConst::LOG_ROTATE_TIME);
        }

        if (logRotateTimeStr[logRotateTimeStr.length() - 1] != 'h' &&
            logRotateTimeStr[logRotateTimeStr.length() - 1] != 'H') {
            throw NacosException(NacosException::INVALID_CONFIG_PARAM,
                                 "Invalid option " + logRotateTimeStr + " for " + PropertyKeyConst::LOG_ROTATE_TIME + ", we only support hour currently");
        }

        int logRotateTime = atol(logRotateTimeStr.substr(0, logRotateTimeStr.length() - 2).c_str());
        if (logRotateTime <= 0) {
            throw NacosException(NacosException::INVALID_CONFIG_PARAM,
                                 PropertyKeyConst::LOG_ROTATE_TIME + " should be greater than 0");
        }
        Logger::setRotateTime(logRotateTime);
    }

    log_info("DEFAULT_LOG_PATH:%s\n", Logger::getBaseDir().c_str());
}

void Logger::Init() {
    initializeLogSystem();
}

}//namespace nacos
