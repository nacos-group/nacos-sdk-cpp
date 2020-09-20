#ifndef __CLIENT_WORKER_H_
#define __CLIENT_WORKER_H_

#include <map>
#include <vector>
#include <pthread.h>
#include "NacosString.h"
#include "http/HttpAgent.h"
#include "listen/Listener.h"
#include "listen/ListeningData.h"
#include "listen/OperateItem.h"
#include "NacosExceptions.h"

/**
 * ClientWorker
 *
 * @author Liu, Hanyu
 * Directly migrated from Java, but with significant work of rewrite/redesign
 */
class ClientWorker {
private:
    //This list holds the listeners to be removed after a performWatch() event
    //And after all the items in this list is removed, an extra check must be performed on listeningKeys
    //to make sure if the entry of that key is empty, remove the entry
    std::list <OperateItem> deleteList;
    //dataID||group||tenant -> Cachedata* Mapping
    std::map<NacosString, ListeningData *> listeningKeys;
    pthread_mutex_t watchListMutex;
    HttpAgent *httpAgent = NULL;
    //Listener thread related info
    pthread_t threadId;

    volatile bool stopThread;
    pthread_mutex_t stopThreadMutex;

    static void *listenerThread(void *watcher);

    //You just can't construct a ClientWorker object without any parameter
    ClientWorker();

    std::vector <NacosString> parseListenedKeys(const NacosString &ReturnedKeys);

    NacosString checkListenedKeys();

    void clearDeleteList(int maxRemoves);

    void cleanUp();

    void addDeleteItem(const OperateItem &item);

public:
    ClientWorker(HttpAgent *_httpAgent);

    ~ClientWorker();

    void startListening();

    void stopListening();

    void addListener(const NacosString &dataId,
                     const NacosString &group,
                     const NacosString &tenant,
                     const NacosString &initialContent,
                     Listener *listener);

    void removeListenerActively(const NacosString &dataId,
                                const NacosString &group,
                                const NacosString &tenant,
                                Listener *listener);

    void removeListener(const NacosString &dataId,
                        const NacosString &group,
                        const NacosString &tenant,
                        Listener *listener);

    void performWatch();

    NacosString getServerConfig(const NacosString &tenant, const NacosString &dataId, const NacosString &group,
                                long timeoutMs) throw(NacosException);
};

#endif