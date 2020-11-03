#ifndef __INSTANCE_H_
#define __INSTANCE_H_

#include <map>
#include "NacosString.h"

namespace nacos{

//a service instance
class Instance {
public:
    Instance & operator = (const Instance &rhs);
    bool operator == (const Instance &rhs) const;
    bool operator != (const Instance &rhs) const;

    Instance();

    /**
    * unique id of this instance.
    */
    NacosString instanceId;

    /**
    * instance ip
    */
    NacosString ip;

    /**
    * instance port
    */
    int port;

    /**
    * instance weight
    */
    double weight;

    /**
    * instance health status
    */
    bool healthy;

    /**
    * If instance is enabled to accept request
    */
    bool enabled;

    /**
    * If instance is ephemeral
    *
    * @since 1.0.0
    */
    bool ephemeral;

    /**
    * cluster information of instance
    */
    NacosString clusterName;

    /**
    * Service information of instance
    */
    NacosString serviceName;

    NacosString groupName;

    NacosString namespaceId;

    /**
    * user extended attributes
    */
    std::map <NacosString, NacosString> metadata;

    NacosString toString() const;
    NacosString toInetAddr();
};
}//namespace nacos

#endif