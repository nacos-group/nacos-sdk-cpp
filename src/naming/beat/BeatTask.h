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
    bool _scheduled;
    uint64_t _interval;//interval for heartbeat got from the server
public:
    BeatTask(BeatInfo &beatInfo, ObjectConfigData *objectConfigData);

    ~BeatTask();

    void setBeatInfo(const BeatInfo &beatInfo);
    BeatInfo getBeatInfo() const;

    void run();

    void setScheduled(bool scheduled) { _scheduled = scheduled; };
    bool getScheduled() { return _scheduled; };

    void setInterval(uint64_t interval) { _interval = interval; };
    uint64_t getInterval() const { return _interval; };
};
}//namespace nacos

#endif