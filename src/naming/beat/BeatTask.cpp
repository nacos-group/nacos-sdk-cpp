#include <map>
#include "BeatReactor.h"
#include "BeatTask.h"
#include "NacosString.h"
#include "Debug.h"

using namespace std;

namespace nacos{
BeatTask::BeatTask(BeatInfo &beatInfo, NamingProxy *namingProxy, BeatReactor *beatReactor)
        : _beatInfo(beatInfo), _namingProxy(namingProxy), _beatReactor(beatReactor), _scheduled(false) {
    incRef();
};

BeatInfo BeatTask::getBeatInfo() const {
    return _beatInfo;
}

void BeatTask::setBeatInfo(const BeatInfo &beatInfo) {
    _beatInfo = beatInfo;
}

void BeatTask::run() {
    long newInterval = _namingProxy->sendBeat(_beatInfo);
    _beatReactor->setClientBeatInterval(newInterval);
    setScheduled(false);
    int refcount = decRef();
    if (refcount == 0) {
        log_debug("[BeatTask]:refCount is 0, deleting this object\n");
        delete this;
    }
}

BeatTask::~BeatTask() {
    NacosString taskName = getTaskName();
    log_debug("[BeatTask]Removing taskObject:%s\n", taskName.c_str());
}
}//namespace nacos