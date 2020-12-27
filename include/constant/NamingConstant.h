#ifndef __NAMING_CONSTANT_H_
#define __NAMING_CONSTANT_H_

#include "NacosString.h"

namespace nacos{
class NamingConstant {
public:
    static const NacosString SERVICE_NAME;

    static const NacosString CLUSTER_NAME;
    static const NacosString UDP_PORT;
    static const NacosString CLUSTERS;
    static const NacosString CLIENT_IP;
    static const NacosString HEALTHY_ONLY;

    static const NacosString HEALTHY;

    static const NacosString NAMESPACE_ID;

    static const NacosString GROUP_NAME;

    static const NacosString SPLITER;

    static const NacosString EMPTY;

    static const NacosString ALL_IPS;

    static const NacosString BEAT;

    static const NacosString PAGE_SIZE;
    static const NacosString PAGE_NO;
};
}//namespace nacos

#endif