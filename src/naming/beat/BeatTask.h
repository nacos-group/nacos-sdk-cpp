#ifndef __BEAT_TASK_H_
#define __BEAT_TASK_H_

#include "BeatInfo.h"
#include "src/naming/NamingProxy.h"
#include "BeatReactor.h"
#include "src/thread/ThreadPool.h"
#include "src/thread/Task.h"
#include "thread/AtomicInt.h"
#include "src/log/Logger.h"
#include "src/factory/ObjectConfigData.h"

namespace nacos{
class BeatReactor;

class BeatTask : public Task {
private:
    BeatInfo _beatInfo;
    ObjectConfigData *_objectConfigData;
    AtomicInt _refCount;
    bool _scheduled;
public:
    BeatTask(BeatInfo &beatInfo, ObjectConfigData *objectConfigData);

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