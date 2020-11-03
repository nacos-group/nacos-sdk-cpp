#ifndef __SVC_INFO_2_H_
#define __SVC_INFO_2_H_

#include <list>
#include "NacosString.h"
#include "naming/Cluster.h"

namespace nacos{ namespace naming {

class Selector {
private:
    NacosString type;
public:

    NacosString getType() const { return type; };

    void setType(const NacosString &_type) { type = _type; };

    virtual NacosString getSelectorString() = 0;
};
} /*naming*/

//Actually this should be exactly the same as ServiceInfo but I designed this for compatibility
//If you check the API reference in detail you'll find that:
//the ServiceInfo returned by /nacos/v1/ns/service differs from the one returned by /nacos/v1/ns/instance/list
//for more details, please refer :
//https://nacos.io/zh-cn/docs/open-api.html#2.4  (Query instances)
//https://nacos.io/zh-cn/docs/open-api.html#2.11 (Query service list)
class ServiceInfo2 {
private:
    bool nullObj;

    bool namespaceIdIsSet;
    NacosString namespaceId;

    bool groupNameIsSet;
    NacosString groupName;

    bool nameIsSet;
    NacosString name;

    bool selectorIsSet;
    NacosString selector;

    bool protectThresholdIsSet;
    double protectThreshold;

    bool clustersIsSet;
    std::list<Cluster> clusters;

    bool metadataIsSet;
    std::map<NacosString, NacosString> metadata;
public:
    static ServiceInfo2 nullServiceInfo2;
    ServiceInfo2() {
        nullObj = true;
        namespaceIdIsSet = false;
        groupNameIsSet = false;
        nameIsSet = false;
        selectorIsSet = false;
        protectThresholdIsSet = false;
        clustersIsSet = false;
        metadataIsSet = false;
    }

    void setNull(bool _isNull) { nullObj = _isNull; };
    bool isNullObject() const { return nullObj; };

    const NacosString &getNamespaceId() const {
        return namespaceId;
    }

    void setNamespaceId(const NacosString &namespaceId) {
        namespaceIdIsSet = true;
        ServiceInfo2::namespaceId = namespaceId;
    }

    const NacosString &getGroupName() const {
        return groupName;
    }

    void setGroupName(const NacosString &groupName) {
        groupNameIsSet = true;
        ServiceInfo2::groupName = groupName;
    }

    const NacosString &getName() const {
        return name;
    }

    void setName(const NacosString &name) {
        nameIsSet = true;
        ServiceInfo2::name = name;
    }

    const NacosString &getSelector() const {
        return selector;
    }

    void setSelector(const NacosString &aSelector) {
        selectorIsSet = true;
        selector = aSelector;
    }

    int getProtectThreshold() const {
        return protectThreshold;
    }

    void setProtectThreshold(double protectThreshold) {
        protectThresholdIsSet = true;
        ServiceInfo2::protectThreshold = protectThreshold;
    }

    const std::list<Cluster> &getClusters() const {
        return clusters;
    }

    void setClusters(const std::list<Cluster> &clusters) {
        clustersIsSet = true;
        ServiceInfo2::clusters = clusters;
    }

    const std::map<std::string, std::string> &getMetadata() const {
        return metadata;
    }

    void setMetadata(const std::map<std::string, std::string> &metadata) {
        metadataIsSet = true;
        ServiceInfo2::metadata = metadata;
    }

    bool isNamespaceIdSet() const {
        return namespaceIdIsSet;
    }

    bool isGroupNameSet() const {
        return groupNameIsSet;
    }

    bool isNameSet() const {
        return nameIsSet;
    }

    bool isSelectorSet() const {
        return selectorIsSet;
    }

    bool isProtectThresholdSet() const {
        return protectThresholdIsSet;
    }

    bool isClustersSet() const {
        return clustersIsSet;
    }

    bool isMetadataSet() const {
        return metadataIsSet;
    }
};
}/*namespace nacos*/

#endif