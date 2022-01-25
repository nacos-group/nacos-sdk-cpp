#include "SubscriptionPoller.h"
#include "constant/ConfigConstant.h"
#include "src/utils/NamingUtils.h"
#include "src/json/JSON.h"
#include "HostReactor.h"

using namespace std;

namespace nacos{
SubscriptionPoller::SubscriptionPoller(ObjectConfigData *objectConfigData)
{
    _objectConfigData = objectConfigData;
    _pollingThread = new Thread("NamingServicePoller", pollingThreadFunc, (void*)this);
    _pollingInterval = atoi(_objectConfigData->_appConfigManager->get(PropertyKeyConst::SUBSCRIPTION_POLL_INTERVAL).c_str());
    _udpPort = atoi(_objectConfigData->_appConfigManager->get(PropertyKeyConst::UDP_RECEIVER_PORT).c_str());
    _started = false;
}

SubscriptionPoller::~SubscriptionPoller()
{
    if (_started) {
        stop();
    }
    if (_pollingThread != NULL)
    {
        delete _pollingThread;
        _pollingThread = NULL;
    }
}

bool SubscriptionPoller::addPollItem(const NacosString &serviceName, const NacosString &groupName, const NacosString &clusters)
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

bool SubscriptionPoller::removePollItem(const NacosString &serviceName, const NacosString &groupName, const NacosString &clusters)
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

void SubscriptionPoller::start()
{
    if (_started) {
        log_warn("Calling start on already-started SubscriptionPoller\n");
        return;
    }
    _started = true;
    _pollingThread->start();
}

void SubscriptionPoller::stop()
{
    if (!_started) {
        log_warn("Calling stop on already-stopped SubscriptionPoller\n");
        return;
    }

    _started = false;
    _pollingThread->kill();
    _pollingThread->join();
}

void *SubscriptionPoller::pollingThreadFunc(void *parm)
{
    SubscriptionPoller *thisObj = (SubscriptionPoller*)parm;
    while (thisObj->_started) {
        log_debug("SubscriptionPoller::pollingThreadFunc start polling, interval = %d\n", thisObj->_pollingInterval);
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
                result = thisObj->_objectConfigData->_serverProxy->queryList(
                        it->second.serviceName, it->second.groupName, it->second.clusters, thisObj->_udpPort,false);
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

            thisObj->_objectConfigData->_hostReactor->processServiceJson(result);
        }

        log_debug("Polling process finished, hibernating...\n");
        sleep(thisObj->_pollingInterval / 1000);
    }
    log_debug("Polling thread for NamingService exited normally.\n");
    return NULL;
}
}//namespace nacos
