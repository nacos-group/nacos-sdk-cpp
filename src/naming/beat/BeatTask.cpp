#include <map>
#include "BeatReactor.h"
#include "NacosString.h"

using namespace std;

namespace nacos{
BeatTask::BeatTask(BeatInfo &beatInfo, ObjectConfigData *objectConfigData)
        : _beatInfo(beatInfo), _objectConfigData(objectConfigData), _scheduled(false) {
};

BeatInfo BeatTask::getBeatInfo() const {
    return _beatInfo;
}

void BeatTask::setBeatInfo(const BeatInfo &beatInfo) {
    _beatInfo = beatInfo;
}

void BeatTask::run() {
    if (!_scheduled) {
        delete this;
        return;
    }
    uint64_t now_ms = TimeUtils::getCurrentTimeInMs();
    _objectConfigData->_beatReactor->_delayedThreadPool->schedule(this, now_ms + _interval);
    _interval = _objectConfigData->_serverProxy->sendBeat(_beatInfo);
}

BeatTask::~BeatTask() {
    NacosString taskName = getTaskName();
    log_debug("[BeatTask]Removing taskObject:%s\n", taskName.c_str());
}
}//namespace nacos