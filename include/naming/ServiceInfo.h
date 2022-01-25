#ifndef __SVC_INFO_H_
#define __SVC_INFO_H_

#include <list>
#include "NacosString.h"
#include "naming/Instance.h"

namespace nacos{
class ServiceInfo {
private:
    //@JSONField(serialize = false)
    NacosString _jsonFromServer;

    NacosString _name;

    NacosString _groupName;

    NacosString _clusters;

    long _cacheMillis;

    //@JSONField(name = "hosts")
    std::list <Instance> _hosts;

    long _lastRefTime;

    NacosString _checksum;

    volatile bool _allIPs;

public:
    ServiceInfo();

    bool isAllIPs() const;

    void setAllIPs(bool allIPs);

    explicit ServiceInfo(const NacosString &key);

    ServiceInfo(const NacosString &name, const NacosString &clusters);

    int ipCount();

    bool expired() const;

    void setHosts(std::list <Instance> &hosts);

    bool isValid();

    NacosString getName();

    void setName(const NacosString &name);

    NacosString getGroupName();

    void setGroupName(const NacosString &groupName);

    void setLastRefTime(long lastRefTime);

    long getLastRefTime();

    NacosString getClusters();

    void setClusters(const NacosString &clusters);

    long getCacheMillis();

    void setCacheMillis(long cacheMillis);

    std::list <Instance> getHosts();

    std::list <Instance> *getHostsNocopy();

    bool validate() const;

    //@JSONField(serialize = false)
    NacosString getJsonFromServer() const;

    void setJsonFromServer(const NacosString &jsonFromServer);

    //@JSONField(serialize = false)
    NacosString getKey() const;

    //@JSONField(serialize = false)
    NacosString getKeyEncoded() const;

    //@JSONField(serialize = false)
    static void fromKey(ServiceInfo &serviceInfo, const NacosString &key);

    //@JSONField(serialize = false)
    static NacosString getKey(const NacosString &name, const NacosString &clusters);

    //@Override
    NacosString toString() const;

    //!!BE CAREFUL!!
    //This function is very expensive!! call it with care!
    NacosString toInstanceString() const;

    NacosString getChecksum() const;

    void setChecksum(const NacosString &checksum);
};
}//namespace nacos

#endif