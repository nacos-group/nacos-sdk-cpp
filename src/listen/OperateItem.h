#ifndef __OPERATE_ITM_H_
#define __OPERATE_ITM_H_

#include "NacosString.h"
#include "listen/Listener.h"

/**
 * ListeningData
 *
 * @author Liu, Hanyu
 * Keeps track of the items to be operated on
 */
namespace nacos{
class OperateItem {
private:
    NacosString tenant;
    NacosString dataId;
    NacosString group;
    Listener *listener;
public:
    NacosString getDataId() const {
        return dataId;
    }

    NacosString getTenant() const {
        return tenant;
    }

    NacosString getGroup() const {
        return group;
    }

    Listener *getListener() const {
        return listener;
    }

    void setListener(Listener *listener) {
        this->listener = listener;
    }

    OperateItem(const NacosString &dataId,
                const NacosString &tenant,
                const NacosString &group,
                Listener *listener) {
        this->dataId = dataId;
        this->tenant = tenant;
        this->group = group;
        this->listener = listener;
    }

    NacosString toString() const {
        return tenant + ":" + group + ":" + dataId;
    }
};
}//namespace nacos

#endif