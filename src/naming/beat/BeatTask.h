#ifndef __BEAT_TASK_H_
#define __BEAT_TASK_H_

#include "BeatInfo.h"
#include "src/naming/NamingProxy.h"
#include "BeatReactor.h"
#include "src/thread/ThreadPool.h"
#include "thread/AtomicInt.h"
#include "Debug.h"

namespace nacos{
class BeatReactor;

class BeatTask : public Task {
private:
    BeatInfo _beatInfo;
    NamingProxy *_namingProxy;
    BeatReactor *_beatReactor;
    AtomicInt _refCount;
    bool _scheduled;
public:
    BeatTask(BeatInfo &beatInfo, NamingProxy *namingProxy, BeatReactor *beatReactor);

    ~BeatTask();

    void setBeatInfo(const BeatInfo &beatInfo);
    BeatInfo getBeatInfo() const;

    int incRef() { return _refCount.inc(); };

    int decRef() { return _refCount.dec(); };

    int getRef() { return _refCount.get(); };

    void run();

    void setScheduled(bool scheduled) { _scheduled = scheduled; };

    bool getScheduled() { return _scheduled; };
};
}//namespace nacos

#endif