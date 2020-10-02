//
// Created by liuhanyu on 2020/9/26.
//

#ifndef NACOS_SDK_CPP_TCPNAMINGSERVICEPOLLER_H
#define NACOS_SDK_CPP_TCPNAMINGSERVICEPOLLER_H

#include <map>
#include "thread/Thread.h"
#include "naming/NamingProxy.h"
#include "EventDispatcher.h"

struct PollingData
{
    NacosString serviceName;
    NacosString groupName;
    NacosString clusters;
};

class TcpNamingServicePoller
{
private:
    Thread *_pollingThread = NULL;
    int _pollingInterval;//In ms
    bool _started;
    EventDispatcher *_eventDispatcher = NULL;
    NamingProxy *_namingProxy = NULL;
    AppConfigManager *_appConfigMgr = NULL;

    TcpNamingServicePoller();

    static void *pollingThreadFunc(void *parm);

    RWLock rwLock;
    std::map<NacosString, PollingData> pollingList;
    std::map<NacosString, ServiceInfo> serviceInfoList;
public:
    TcpNamingServicePoller(EventDispatcher *eventDispatcher, NamingProxy *namingProxy, AppConfigManager *appConfigManager);
    bool addPollItem(const NacosString &serviceName, const NacosString &groupName, const NacosString &clusters);
    bool removePollItem(const NacosString &serviceName, const NacosString &groupName, const NacosString &clusters);
    void start();
    void stop();
    ~TcpNamingServicePoller();

};

#endif //NACOS_SDK_CPP_TCPNAMINGSERVICEPOLLER_H
