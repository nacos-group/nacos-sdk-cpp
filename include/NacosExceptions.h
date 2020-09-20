#ifndef __EXCEPTIONS_H_
#define __EXCEPTIONS_H_

#include <exception>
#include "NacosString.h"

class NacosException : public std::exception {
private:
    int _errcode;
    NacosString _errmsg;
public:
    NacosException(int errorcode, const char *errormsg) throw();

    NacosException(int errorcode, const NacosString &errormsg) throw();

    ~NacosException() throw() {};

    const char *what() const throw() { return _errmsg.c_str(); };

    const int errorcode() const throw() { return _errcode; };

    static const int CLIENT_INVALID_PARAM = -400;
    /**
    * over client threshold（超过server端的限流阈值）
    */
    static const int CLIENT_OVER_THRESHOLD = -503;

    /**
    * server error code
    * 400 403 throw exception to user
    * 500 502 503 change ip and retry
    */

    /**
    * invalid param（参数错误）
    */
    static const int INVALID_PARAM = 400;
    /**
    * no right（鉴权失败）
    */
    static const int NO_RIGHT = 403;
    /**
    * conflict（写并发冲突）
    */
    static const int CONFLICT = 409;
    /**
    * server error（server异常，如超时）
    */
    static const int SERVER_ERROR = 500;
    /**
    * bad gateway（路由异常，如nginx后面的Server挂掉）
    */
    static const int BAD_GATEWAY = 502;
    /**
    * over threshold（超过server端的限流阈值）
    */
    static const int OVER_THRESHOLD = 503;

    /**
    * Error while parsing the JSON string
    */
    static const int INVALID_JSON_FORMAT = 100001;

    /**
    * One or more JSON field is missing
    */
    static const int LACK_JSON_FIELD = 100002;

    static const int MALFORMED_CONFIG_FILE = 1001;
    static const int FILE_NOT_FOUND = 1002;
    static const int INVALID_FACTORY_CONFIG = 1003;
};

class NetworkException : public std::exception {
private:
    int _curlerrcode;
    NacosString _errmsg;
public:
    NetworkException(int errorcode, const char *errormsg) throw(): _curlerrcode(errorcode), _errmsg(errormsg) {};

    NetworkException(int errorcode, const NacosString &errormsg) throw(): _curlerrcode(errorcode), _errmsg(errormsg) {};

    ~NetworkException() throw() {};

    const char *what() const throw() { return _errmsg.c_str(); };

    const int errorcode() const throw() { return _curlerrcode; };
};

class IOException : public std::exception {
private:
    int _errcode;
    NacosString _errmsg;
public:
    IOException(int errorcode, const char *errormsg) throw(): _errcode(errorcode), _errmsg(errormsg) {};

    IOException(int errorcode, const NacosString &errormsg) throw(): _errcode(errorcode), _errmsg(errormsg) {};

    ~IOException() throw() {};

    const char *what() const throw() { return _errmsg.c_str(); };

    const int errorcode() const throw() { return _errcode; };
};

class MalformedConfigException : public NacosException {
public:
    MalformedConfigException(const NacosString &file) : NacosException(NacosException::MALFORMED_CONFIG_FILE,
                                                                       "Malformed Config file:" + file) {};
};

class InvalidFactoryConfigException : public NacosException {
public:
    InvalidFactoryConfigException() : NacosException(NacosException::INVALID_FACTORY_CONFIG,
                                                     "Either config file or property should be configed.") {};
};

#endif
