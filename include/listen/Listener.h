#ifndef __LISTENER_H_
#define __LISTENER_H_
#include "NacosString.h"

class Listener
{
private:
    //Will be removed during next listening process if true
    bool remove;
    NacosString listenerName;
public:
    Listener()
    {
        remove = false;
        this->listenerName = "theListener";
    };
    bool isRemoving() const { return remove; };
    void setRemove(bool remove) { this->remove = remove; };
    void setListenerName(const NacosString &name) { this->listenerName = name; };
    NacosString getListenerName() const { return listenerName; };
	virtual void receiveConfigInfo(const NacosString &configInfo) = 0;
	virtual ~Listener() {};
};

#endif