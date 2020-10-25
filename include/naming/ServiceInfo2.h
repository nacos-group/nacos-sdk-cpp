#ifndef __SVC_INFO_2_H_
#define __SVC_INFO_2_H_

#include <list>
#include "NacosString.h"
#include "naming/Cluster.h"

namespace nacos{

class Selector {
private:
    NacosString type;
public:

    NacosString getType() const { return type; };

    void setType(const NacosString &_type) { type = _type; };
};

//Actually this should be exactly the same as ServiceInfo but I designed this for compatibility
//If you check the API reference in detail you'll find that:
//the ServiceInfo returned by /nacos/v1/ns/service differs from the one returned by /nacos/v1/ns/instance/list
//for more details, please refer :
//https://nacos.io/zh-cn/docs/open-api.html#2.4  (Query instances)
//https://nacos.io/zh-cn/docs/open-api.html#2.11 (Query service list)
class ServiceInfo2 {
private:
    bool nullObj;

    NacosString namespaceId;

    NacosString groupName;

    NacosString name;

    Selector selector;

    int protectThreshold;

    std::list<Cluster> clusters;

    std::map<NacosString, NacosString> metadata;
public:

    void setNull(bool _isNull) { nullObj = _isNull; };
    bool isNullObject() const { return nullObj; };

    NacosString getNamespaceId() const {
        return namespaceId;
    }

    void setNamespaceId(const NacosString &namespaceId) {
        ServiceInfo2::namespaceId = namespaceId;
    }

    NacosString getGroupName() const {
        return groupName;
    }

    void setGroupName(const NacosString &groupName) {
        ServiceInfo2::groupName = groupName;
    }

    NacosString getName() const {
        return name;
    }

    void setName(const NacosString &name) {
        ServiceInfo2::name = name;
    }

    Selector getSelector() const {
        return selector;
    }

    void setSelector(const Selector &aSelector) {
        selector = aSelector;
    }

    int getProtectThreshold() const {
        return protectThreshold;
    }

    void setProtectThreshold(int protectThreshold) {
        ServiceInfo2::protectThreshold = protectThreshold;
    }

    std::list<Cluster> getClusters() const {
        return clusters;
    }

    void setClusters(const std::list<Cluster> &clusters) {
        ServiceInfo2::clusters = clusters;
    }

    std::map<std::string, std::string> getMetadata() const {
        return metadata;
    }

    void setMetadata(const std::map<std::string, std::string> &metadata) {
        ServiceInfo2::metadata = metadata;
    }
};
}//namespace nacos

#endif