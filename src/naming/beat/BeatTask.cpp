#include <map>
#include "BeatReactor.h"
#include "BeatTask.h"
#include "NacosString.h"
#include "src/log/Logger.h"
#include "src/utils/TimeUtils.h"

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
        _objectConfigData->_beatReactor->removeBeatInfo(_beatInfo.serviceName, _beatInfo.ip, _beatInfo.port);
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