#include "TcpNamingServicePoller.h"
#include "Constants.h"
#include "utils/NamingUtils.h"
#include "src/json/JSON.h"

using namespace std;

namespace nacos{
TcpNamingServicePoller::TcpNamingServicePoller(EventDispatcher *eventDispatcher, NamingProxy *namingProxy, AppConfigManager *appConfigManager)
{
    _eventDispatcher = eventDispatcher;
    _namingProxy = namingProxy;
    _appConfigMgr = appConfigManager;
    _pollingThread = new Thread("NamingServicePoller", pollingThreadFunc, (void*)this);
    _pollingInterval = atoi(_appConfigMgr->get(PropertyKeyConst::TCP_NAMING_POLL_INTERVAL).c_str());
    _started = false;
}

TcpNamingServicePoller::~TcpNamingServicePoller()
{
    if (_started) {
        stop();
    }
    _eventDispatcher = NULL;
    _namingProxy = NULL;
    _appConfigMgr = NULL;
    if (_pollingThread != NULL)
    {
        delete _pollingThread;
        _pollingThread = NULL;
    }
}

bool TcpNamingServicePoller::addPollItem(const NacosString &serviceName, const NacosString &groupName, const NacosString &clusters)
{
    struct PollingData pd;
    pd.clusters = clusters;
    pd.serviceName = serviceName;
    pd.groupName = groupName;
    NacosString name = NamingUtils::getGroupedName(serviceName, groupName);
    NacosString key = ServiceInfo::getKey(name, clusters);

    {
        WriteGuard __writeGuard(rwLock);
        if (pollingList.count(key) > 0) {
            return false;
        }
        pollingList[key] = pd;
        return true;
    }
}

bool TcpNamingServicePoller::removePollItem(const NacosString &serviceName, const NacosString &groupName, const NacosString &clusters)
{
    NacosString name = NamingUtils::getGroupedName(serviceName, groupName);
    NacosString key = ServiceInfo::getKey(name, clusters);
    {
        WriteGuard __writeGuard(rwLock);
        if (pollingList.count(key) == 0) {
            return false;
        }
        pollingList.erase(key);
    }
    return true;
}

void TcpNamingServicePoller::start()
{
    if (_started) {
        log_warn("Calling start on already-started TcpNamingServicePoller\n");
        return;
    }
    _started = true;
    _pollingThread->start();
}

void TcpNamingServicePoller::stop()
{
    if (!_started) {
        log_warn("Calling stop on already-stopped TcpNamingServicePoller\n");
        return;
    }

    _started = false;
    _pollingThread->join();
}

void *TcpNamingServicePoller::pollingThreadFunc(void *parm)
{
    TcpNamingServicePoller *thisObj = (TcpNamingServicePoller*)parm;
    while (thisObj->_started) {
        log_debug("TcpNamingServicePoller::pollingThreadFunc start polling, interval = %d\n", thisObj->_pollingInterval);
        map<NacosString, PollingData> copiedList;
        {
            ReadGuard __readGuard(thisObj->rwLock);
            copiedList = thisObj->pollingList;
            log_debug("Copied polling list, size = %d\n", copiedList.size());
        }
        if (copiedList.empty()) {
            log_debug("PollingList is empty, hibernating...\n", copiedList.size());
            sleep(thisObj->_pollingInterval / 1000);
            continue;
        }

        for (map<NacosString, PollingData>::iterator it = copiedList.begin();
             it != copiedList.end(); it++) {
            NacosString name = NamingUtils::getGroupedName(it->second.serviceName, it->second.groupName);
            NacosString key = ServiceInfo::getKey(name, it->second.clusters);
            log_debug("Polling data: name=%s, key=%s\n", name.c_str(), key.c_str());

            NacosString result;
            try {
                result = thisObj->_namingProxy->queryList(it->second.serviceName, it->second.clusters, 0,
                                                          false);
            }
            catch (NacosException &e) {
                //no server available or all servers tried but failed
                //just ignore and wait the network restore or manual restore
                //the reason why we choose to ignore the exception is that:
                //1. the network is down, we don't know the current status of the server, so we don't know whether it changed or not
                //for that reason we can't make any notifications
                //2. when the network restored, we can get the latest status of the server and send notifications
                //3. if the network is down for a rather long time, manual restore is needed,
                //and the server will be restarted, this listener thread will be restarted, too
                //4. when the server returns a 404 error, we still need to poll the service's information, since it may become available sometime later
                continue;
            }

            log_debug("Server info got from server:%s\n=======>\n", result.c_str());
            ServiceInfo serviceInfo = JSON::JsonStr2ServiceInfo(result);
            if (thisObj->serviceInfoList.count(key) == 0) {
                //The first time, don't notify
                thisObj->serviceInfoList[key] = serviceInfo;
            } else {
                ServiceInfo oldServiceInfo = thisObj->serviceInfoList[key];
                ChangeAdvice changeAdvice;
                changeAdvice.key = key;
                ChangeAdvice::compareChange(oldServiceInfo, serviceInfo, changeAdvice);
                log_debug("Change status:modified:%d added:%d removed:%d\n", changeAdvice.modified, changeAdvice.added, changeAdvice.removed);
                if (changeAdvice.modified || changeAdvice.added || changeAdvice.removed) {
                    //asm volatile("int $3");
                    changeAdvice.newServiceInfo = serviceInfo;
                    thisObj->_eventDispatcher->notifyDirectly(changeAdvice);
                }
                thisObj->serviceInfoList[key] = serviceInfo;//update local service info to the new one
            }
        }

        log_debug("Polling process finished, hibernating...\n");
        sleep(thisObj->_pollingInterval / 1000);
    }
    log_debug("Polling thread for NamingService exited normally.\n");
    return NULL;
}
}//namespace nacos
