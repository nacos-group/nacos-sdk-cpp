#include <map>
#include "BeatReactor.h"
#include "BeatTask.h"
#include "NacosString.h"
#include "src/debug/DebugAssertion.h"

using namespace std;

namespace nacos{
void BeatReactor::start() {
    _stop = false;
    _delayedThreadPool->start();
}

void BeatReactor::stop() {
    if (_stop) {
        log_warn("Calling BeatReactor::stop() on an already-stopped BeatReactor\n");
        return;
    }
    _stop = true;
    _delayedThreadPool->stop();
}

void BeatReactor::addBeatInfo(const NacosString &serviceName, BeatInfo &beatInfo) {
    NacosString beatInfoStr = beatInfo.toString();
    log_info("[BEAT] adding beat: %s to beat map.\n", beatInfoStr.c_str());
    NacosString beatKey = buildKey(serviceName, beatInfo.ip, beatInfo.port);
    BeatTask *newBeatTask;
    {
        WriteGuard _lockguard(_beatInfoLock);
        //The specified beatInfo is already in the list
        if (_beatInfoList.count(beatKey) != 0) {
            log_warn("Adding already-exist key:%s\n", beatKey.c_str());
            return;
        }
        newBeatTask = new BeatTask(beatInfo, _objectConfigData);
        newBeatTask->setScheduled(true);
        newBeatTask->setTaskName(beatKey);
        newBeatTask->setInterval(_clientBeatInterval);
        _beatInfoList[beatKey] = newBeatTask;
    }
    _delayedThreadPool->schedule(newBeatTask, TimeUtils::getCurrentTimeInMs() + _clientBeatInterval);
    //TODO:MetricsMonitor.getDom2BeatSizeMonitor().set(dom2Beat.size());
}

/**
* Looks up a beatInfo and return it via beatInfo
* @param serviceName the service name
* @param ip the service's ip
* @param port the service's port
* @param beatInfo the beatInfo, ip and port must be set and cannot be changed via this function
* @return false if beatInfo can't be found with designated serviceName, ip and port
*         true if beatInfo is found
 *        the BeatInfo is returned via beatInfo parameter
*/
bool BeatReactor::getBeatInfo(const NacosString &serviceName, const NacosString &ip, int port, BeatInfo &beatInfo) {
    NacosString beatKey = buildKey(serviceName, ip, port);
    {
        ReadGuard _lockguard(_beatInfoLock);
        if (_beatInfoList.count(beatKey) == 0) {
            return false;
        }

        beatInfo = _beatInfoList[beatKey]->getBeatInfo();
        return true;
    }
}

/**
* Modifies a beatInfo
* @param serviceName the service name
* @param beatInfo the beatInfo, ip and port must be set and cannot be changed via this function
* @return false if nothing is modified (e.g.: the beatInfo doesn't exist in BeatReactor)
*         true if modification is performed
*/
bool BeatReactor::modifyBeatInfo(const NacosString &serviceName, BeatInfo &beatInfo) {
    NacosString beatInfoStr = beatInfo.toString();
    log_info("[BEAT] modify beat: %s to beat map.\n", beatInfoStr.c_str());
    NacosString beatKey = buildKey(serviceName, beatInfo.ip, beatInfo.port);
    {
        WriteGuard _lockguard(_beatInfoLock);
        if (_beatInfoList.count(beatKey) != 0) {
            log_warn("Modifying non-existent key:%s\n", beatKey.c_str());
            return false;
        }
        BeatInfo originalBeatInfo = _beatInfoList[beatKey]->getBeatInfo();
        originalBeatInfo.weight = beatInfo.weight;
        originalBeatInfo.cluster = beatInfo.cluster;
        originalBeatInfo.metadata = beatInfo.metadata;

        _beatInfoList[beatKey]->setBeatInfo(originalBeatInfo);//modified
    }

    return true;
}

/**
* Removes a beatInfo
* @param serviceName the service name
* @param ip the service's ip
* @param port the service's port
* @return false if nothing is removed (e.g.: the beatInfo doesn't exist in BeatReactor)
*         true if a beatInfo is removed
*/
bool BeatReactor::removeBeatInfo(const NacosString &serviceName, const NacosString &ip, int port) {
    log_info("[BEAT] removing beat: %s:%s:%d from beat map.", serviceName.c_str(), ip.c_str(), port);
    NacosString beatKey = buildKey(serviceName, ip, port);
    BeatTask *beatTaskToRemove = NULL;
    {
        WriteGuard _lockguard(_beatInfoLock);
        //If we can't find the beatInfo in the list, just return
        if (_beatInfoList.count(beatKey) != 1) {
            log_warn("Removing non-existent key:%s\n", beatKey.c_str());
            return false;
        }
        beatTaskToRemove = _beatInfoList[beatKey];
        beatTaskToRemove->setScheduled(false);
        _beatInfoList.erase(beatKey);
    }
    //TODO:MetricsMonitor.getDom2BeatSizeMonitor().set(dom2Beat.size());

    return true;
}

void BeatReactor::removeAllBeatInfo() {
    log_debug("BeatReactor::removeAllBeatInfo() start to remove\n");
    WriteGuard _lockguard(_beatInfoLock);
    for (map<NacosString, BeatTask *>::iterator it = _beatInfoList.begin();
         it != _beatInfoList.end(); it++) {
        BeatTask *curTask = it->second;
        delete curTask;
        curTask = NULL;
    }
    _beatInfoList.clear();
}

NacosString BeatReactor::buildKey(const NacosString &serviceName, const NacosString &ip, int port) {
    return serviceName + ConfigConstant::NAMING_INSTANCE_ID_SPLITTER
           + ip + ConfigConstant::NAMING_INSTANCE_ID_SPLITTER + NacosStringOps::valueOf(port);
}
}//namespace nacos
