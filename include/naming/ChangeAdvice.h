#ifndef __CHG_ADVICE_H_
#define __CHG_ADVICE_H_

#include "NacosString.h"
#include "naming/ServiceInfo.h"

//The wrapper class for service change information
namespace nacos{
class ChangeAdvice {
public:
    bool added;//indicates Instance is added
    bool removed;//indicates Instance is removed
    bool modified;//indicates Instance is modified
    //std::list<Instance> addedInstances;
    //std::list<Instance> removedInstances;
    //std::list<Instance> modifiedInstances;
    //ServiceInfo oldServiceInfo;
    ServiceInfo newServiceInfo;
    NacosString key;
public:
    ChangeAdvice();
    ~ChangeAdvice();
    static void compareChange(ServiceInfo &oldInfo, ServiceInfo &newInfo, ChangeAdvice &changeAdvice);
    NacosString toString();
};
}//namespace nacos

#endif