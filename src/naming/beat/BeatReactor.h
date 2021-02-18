#ifndef __BEAT_REACTOR_H_
#define __BEAT_REACTOR_H_

#include <map>
#include "src/naming/NamingProxy.h"
#include "naming/Instance.h"
#include "NacosString.h"
#include "NacosExceptions.h"
#include "src/thread/DelayedThreadPool.h"
#include "src/thread/Thread.h"
#include "src/thread/RWLock.h"
#include "BeatTask.h"
#include "constant/ConfigConstant.h"
#include "constant/UtilAndComs.h"
#include "src/factory/ObjectConfigData.h"

namespace nacos{
class BeatReactor {
private:
    ObjectConfigData *_objectConfigData;
    int _threadCount;
    RWLock _beatInfoLock;
    std::map<NacosString, BeatTask *> _beatInfoList;
    volatile uint64_t _clientBeatInterval;
protected:
    friend class BeatTask;
    volatile bool _stop;
    DelayedThreadPool *_delayedThreadPool;
public:
    void setClientBeatInterval(uint64_t interval) { _clientBeatInterval = interval; };

    uint64_t getClientBeatInterval() const { return _clientBeatInterval; };

    BeatReactor(ObjectConfigData *objectConfigData, int threadCount)
            : _objectConfigData(objectConfigData), _beatInfoLock() {
        _threadCount = threadCount;
        _stop = true;
        _clientBeatInterval = 5 * 1000;
        _delayedThreadPool = new DelayedThreadPool("BeatReactor-Daemon", threadCount);
    };

    BeatReactor(ObjectConfigData *objectConfigData)
            : _objectConfigData(objectConfigData), _beatInfoLock() {
        _threadCount = UtilAndComs::DEFAULT_CLIENT_BEAT_THREAD_COUNT;
        _stop = true;
        _clientBeatInterval = 5 * 1000;
        _delayedThreadPool = new DelayedThreadPool("BeatReactor-Daemon", _threadCount);
    };

    ~BeatReactor() {
        log_debug("BeatReactor::~BeatReactor() calling stop\n");
        stop();
        log_debug("BeatReactor::~BeatReactor() delete threadpool\n");
        delete _delayedThreadPool;
        _delayedThreadPool = NULL;
        log_debug("BeatReactor::~BeatReactor() removeAllBeatInfo()\n");
        removeAllBeatInfo();
    };

    void start();

    void stop();

    void addBeatInfo(const NacosString &serviceName, BeatInfo &beatInfo);


    bool modifyBeatInfo(const NacosString &serviceName, BeatInfo &beatInfo);

    bool getBeatInfo(const NacosString &serviceName, const NacosString &ip, int port, BeatInfo &beatInfo);

    bool removeBeatInfo(const NacosString &serviceName, const NacosString &ip, int port);

    //NOTICE:Should be invoked ONLY when the working threads are ALL STOPPED
    void removeAllBeatInfo();

    NacosString buildKey(const NacosString &serviceName, const NacosString &ip, int port);
};
}//namespace nacos

#endif