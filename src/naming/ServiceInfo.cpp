#include "naming/ServiceInfo.h"
#include <vector>
#include <list>
#include <sys/time.h>
#include "NacosString.h"
#include "constant/ConfigConstant.h"
#include "src/utils/url.h"
#include "src/utils/ParamUtils.h"
#include "naming/Instance.h"
namespace nacos{
ServiceInfo::ServiceInfo() : _jsonFromServer(""), _cacheMillis(1000L), _lastRefTime(0L), _checksum(""), _allIPs(false) {
}

bool ServiceInfo::isAllIPs() const{
    return _allIPs;
}

void ServiceInfo::setAllIPs(bool allIPs) {
    _allIPs = allIPs;
}

ServiceInfo::ServiceInfo(const NacosString &key)  : _jsonFromServer(""), _cacheMillis(1000L), _lastRefTime(0L), _checksum(""),
                                        _allIPs(false) {
    std::vector <NacosString> segs;
    ParamUtils::Explode(segs, key, ConfigConstant::SERVICE_INFO_SPLITER);

    if (segs.size() == 2) {
        setGroupName(segs[0]);
        setName(segs[1]);
    } else if (segs.size() == 3) {
        setGroupName(segs[0]);
        setName(segs[1]);
        setClusters(segs[2]);
    }
}

ServiceInfo::ServiceInfo(const NacosString &name, const NacosString &clusters) {
    _name = name;
    _clusters = clusters;
}

int ServiceInfo::ipCount() {
    return _hosts.size();
}

bool ServiceInfo::expired() const{
    //TODO:extract this snippet to a common util
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    return ms - _lastRefTime > _cacheMillis;
}

void ServiceInfo::setHosts(std::list <Instance> &hosts) {
    _hosts = hosts;
}

bool ServiceInfo::isValid() {
    return _hosts.size() > 0;
}

NacosString ServiceInfo::getName() {
    return _name;
}

void ServiceInfo::setName(const NacosString &name) {
    _name = name;
}

NacosString ServiceInfo::getGroupName() {
    return _groupName;
}

void ServiceInfo::setGroupName(const NacosString &groupName) {
    _groupName = groupName;
}

void ServiceInfo::setLastRefTime(long lastRefTime) {
    _lastRefTime = lastRefTime;
}

long ServiceInfo::getLastRefTime() {
    return _lastRefTime;
}

NacosString ServiceInfo::getClusters() {
    return _clusters;
}

void ServiceInfo::setClusters(const NacosString &clusters) {
    _clusters = clusters;
}

long ServiceInfo::getCacheMillis() {
    return _cacheMillis;
}

void ServiceInfo::setCacheMillis(long cacheMillis) {
    _cacheMillis = cacheMillis;
}

std::list <Instance> ServiceInfo::getHosts() {
    return _hosts;
}

std::list <Instance> *ServiceInfo::getHostsNocopy() {
    return &_hosts;
}

bool ServiceInfo::validate() const{
    if (isAllIPs()) {
        return true;
    }

    //TODO: Idk what does this mean in Java, ignore in C++
    /*std::list<Instance> validHosts;
    for (std::list<Instance>::iterator it = _hosts.begin()
    it != _hosts.end(); it++)
    {
        if (it->isHealthy())
        {
            continue;
        }

        for (int i = 0; i < it->getWeight(); i++)
        {
            validHosts.push_back(*it);
        }
    }*/

    return true;
}

//@JSONField(serialize = false)
NacosString ServiceInfo::getJsonFromServer() const{
    return _jsonFromServer;
}

void ServiceInfo::setJsonFromServer(const NacosString &jsonFromServer) {
    _jsonFromServer = jsonFromServer;
}

//@JSONField(serialize = false)
NacosString ServiceInfo::getKey() const{
    return getKey(_name, _clusters);
}

//@JSONField(serialize = false)
NacosString ServiceInfo::getKeyEncoded() const{
    return getKey(urlencode(_name), _clusters);
}

//@JSONField(serialize = false)
void ServiceInfo::fromKey(ServiceInfo &serviceInfo, const NacosString &key) {
    std::vector <NacosString> segs;
    ParamUtils::Explode(segs, key, ConfigConstant::SERVICE_INFO_SPLITER);

    if (segs.size() == 2) {
        serviceInfo.setGroupName(segs[0]);
        serviceInfo.setName(segs[1]);
    } else if (segs.size() == 3) {
        serviceInfo.setGroupName(segs[0]);
        serviceInfo.setName(segs[1]);
        serviceInfo.setClusters(segs[2]);
    }
}

//@JSONField(serialize = false)
NacosString ServiceInfo::getKey(const NacosString &name, const NacosString &clusters) {
    if (!ParamUtils::isBlank(clusters)) {
        return name + ConfigConstant::SERVICE_INFO_SPLITER + clusters;
    }

    return name;
}

//@Override
NacosString ServiceInfo::toString() const{
    return getKey();
}

//!!BE CAREFUL!!
//This function is very expensive!! call it with care!
NacosString ServiceInfo::toInstanceString() const{
    NacosString res = "{\"lastRefTime\":" + NacosStringOps::valueOf(_lastRefTime) + " [\n";
    for (std::list<Instance>::const_iterator it = _hosts.begin();
        it != _hosts.end(); it++)
    {
        res += it->toString() + "\n";
    }

    res += "\n]}";

    return res;
}

NacosString ServiceInfo::getChecksum() const{
    return _checksum;
}

void ServiceInfo::setChecksum(const NacosString &checksum) {
    _checksum = checksum;
}

}