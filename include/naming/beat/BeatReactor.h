#ifndef __BEAT_REACTOR_H_
#define __BEAT_REACTOR_H_

#include <map>
#include "naming/NamingProxy.h"
#include "naming/Instance.h"
#include "NacosString.h"
#include "NacosExceptions.h"
#include "thread/ThreadPool.h"
#include "thread/Thread.h"
#include "thread/Mutex.h"
#include "naming/beat/BeatTask.h"
#include "Constants.h"
#include "utils/UtilAndComs.h"

class BeatReactor {
private:
    NamingProxy *_namingProxy;
    int _threadCount;
    ThreadPool *_threadPool;
    Thread *_beatMaster;
    Mutex _beatInfoLock;
    std::map<NacosString, BeatTask *> _beatInfoList;
    volatile bool _stop;
    volatile long _clientBeatInterval;

    static void *beatMaster(void *param);

public:
    void setClientBeatInterval(long interval) { _clientBeatInterval = interval; };

    long getClientBeatInterval() { return _clientBeatInterval; };

    BeatReactor(NamingProxy *namingProxy, int threadCount)
            : _namingProxy(namingProxy), _threadCount(threadCount), _beatInfoLock(), _stop(true),
              _clientBeatInterval(5 * 1000) {
        _threadPool = new ThreadPool("HeartbeatDaemonPool", _threadCount);
        _beatMaster = new Thread("BeatMaster", beatMaster, this);
    };

    BeatReactor(NamingProxy *namingProxy)
            : _namingProxy(namingProxy), _threadCount(UtilAndComs::DEFAULT_CLIENT_BEAT_THREAD_COUNT), _beatInfoLock(),
              _stop(true), _clientBeatInterval(5 * 1000) {
        _threadPool = new ThreadPool("HeartbeatDaemonPool", _threadCount);
        _beatMaster = new Thread("BeatMaster", beatMaster, this);
    };

    ~BeatReactor() {
        stop();
        delete _beatMaster;
        delete _threadPool;
        _threadPool = NULL;
        _beatMaster = NULL;
        removeAllBeatInfo();
    };

    void start();

    void stop();

    void addBeatInfo(const NacosString &serviceName, BeatInfo &beatInfo);

    void removeBeatInfo(const NacosString &serviceName, const NacosString &ip, int port);

    //NOTICE:Should be invoked ONLY when the working threads are ALL STOPPED
    void removeAllBeatInfo();

    NacosString buildKey(const NacosString &serviceName, const NacosString &ip, int port);
};

#endif