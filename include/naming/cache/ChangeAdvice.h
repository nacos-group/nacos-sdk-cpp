#ifndef __CHG_ADVICE_H_
#define __CHG_ADVICE_H_

#include "NacosString.h"
#include "naming/ServiceInfo.h"

enum ChangeType
{
    ADD,
    UPDATE,
    REMOVE
};

class ChangeAdvice {
public:
    bool added;//indicates Instance is added
    bool removed;//indicates Instance is removed
    bool modified;//indicates Instance is modified
    std::list<Instance> addedInstances;
    std::list<Instance> removedInstances;
    std::list<Instance> modifiedInstances;
    ServiceInfo oldServiceInfo;
    ServiceInfo newServiceInfo;
    ChangeType changeType;
public:
    ChangeAdvice();
    ~ChangeAdvice();
    static void compareChange(ServiceInfo &oldInfo, ServiceInfo &newInfo, ChangeAdvice &changeAdvice);
};

#endif