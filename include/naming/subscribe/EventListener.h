//
// Created by liuhanyu on 2020/9/24.
//

#ifndef NACOS_SDK_CPP_EVENTLISTENER_H
#define NACOS_SDK_CPP_EVENTLISTENER_H

#include "NacosString.h"
#include "thread/AtomicInt.h"
#include "naming/ChangeAdvice.h"

namespace nacos{
class EventListener {
private:
    NacosString listenerName;
    AtomicInt<int> refCount;
public:
    EventListener() {
        this->listenerName = "theListener";
    };

    int incRef() { return refCount.inc(); };

    int decRef() { return refCount.dec(); };

    int refCnt() const { return refCount.get(); };

    void setListenerName(const NacosString &name) { this->listenerName = name; };

    NacosString getListenerName() const { return listenerName; };

    virtual void receiveNamingInfo(const ServiceInfo &changeAdvice) = 0;

    virtual ~EventListener();
};
}//namespace nacos


#endif //NACOS_SDK_CPP_EVENTLISTENER_H
