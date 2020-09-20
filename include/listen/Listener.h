#ifndef __LISTENER_H_
#define __LISTENER_H_

#include "NacosString.h"
#include "thread/AtomicInt.h"

class Listener {
private:
    NacosString listenerName;
    AtomicInt refCount;
public:
    Listener() {
        this->listenerName = "theListener";
    };

    int incRef() { return refCount.inc(); };

    int decRef() { return refCount.dec(); };

    int refCnt() const { return refCount.get(); };

    void setListenerName(const NacosString &name) { this->listenerName = name; };

    NacosString getListenerName() const { return listenerName; };

    virtual void receiveConfigInfo(const NacosString &configInfo) = 0;

    virtual ~Listener() {};
};

#endif