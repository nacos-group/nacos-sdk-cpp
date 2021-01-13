//
// Created by liuhanyu on 2020/9/26.
//

#ifndef NACOS_SDK_CPP_SUBSCRIPTIONPOLLER_H
#define NACOS_SDK_CPP_SUBSCRIPTIONPOLLER_H

#include <map>
#include "src/thread/Thread.h"
#include "src/naming/NamingProxy.h"
#include "EventDispatcher.h"
#include "src/factory/ObjectConfigData.h"

namespace nacos{
struct PollingData
{
    NacosString serviceName;
    NacosString groupName;
    NacosString clusters;
    volatile long nextPollTime;
};

class SubscriptionPoller
{
private:
    Thread *_pollingThread;
    int _pollingInterval;//In ms
    int _udpPort;//udp receiver port
    volatile bool _started;
    ObjectConfigData *_objectConfigData;

    SubscriptionPoller();

    static void *pollingThreadFunc(void *parm);

    //for polling list
    RWLock rwLock;
    std::map<NacosString, PollingData> pollingList;
public:
    SubscriptionPoller(ObjectConfigData *objectConfigData);
    bool addPollItem(const NacosString &serviceName, const NacosString &groupName, const NacosString &clusters);
    bool removePollItem(const NacosString &serviceName, const NacosString &groupName, const NacosString &clusters);
    void start();
    void stop();
    ~SubscriptionPoller();

};
}//namespace nacos

#endif //NACOS_SDK_CPP_SUBSCRIPTIONPOLLER_H
