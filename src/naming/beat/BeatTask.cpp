#include <map>
#include "naming/beat/BeatReactor.h"
#include "naming/beat/BeatTask.h"
#include "NacosString.h"
#include "Debug.h"

using namespace std;

BeatTask::BeatTask(BeatInfo &beatInfo, NamingProxy *namingProxy, BeatReactor *beatReactor)
        : _beatInfo(beatInfo), _namingProxy(namingProxy), _beatReactor(beatReactor), _scheduled(false) {
    incRef();
};

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