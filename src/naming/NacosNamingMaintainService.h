#ifndef __NACOS_NAM_MTN_SVC_H_
#define __NACOS_NAM_MTN_SVC_H_

#include "naming/NamingMaintainService.h"
#include "naming/Instance.h"
#include "src/naming/NamingProxy.h"
#include "src/http/IHttpCli.h"
#include "NacosString.h"
#include "Properties.h"
#include "src/factory/ObjectConfigData.h"
#include "Compatibility.h"

namespace nacos{

/**
 * NacosNamingMaintainService
 * Maintain functions for Nacos
 *
 * @author liaochuntao
 * @author Liu, Hanyu
 * Maintain service
 * Special thanks to @liaochuntao
 */
class NacosNamingMaintainService : public NamingMaintainService {
private:
    NacosNamingMaintainService();
    ObjectConfigData *_objectConfigData;
public:
    NacosNamingMaintainService(ObjectConfigData *objectConfigData);
    bool updateInstance(const NacosString &serviceName, const NacosString & groupName, const Instance &instance) NACOS_THROW(NacosException);
    ServiceInfo2 queryService(const NacosString &serviceName, const NacosString &groupName) NACOS_THROW(NacosException);
    bool createService(const ServiceInfo2 &service, naming::Selector *selector) NACOS_THROW(NacosException);
    bool deleteService(const NacosString &serviceName, const NacosString &groupName) NACOS_THROW(NacosException);
    bool updateService(const ServiceInfo2 &service, naming::Selector *selector) NACOS_THROW(NacosException);
    virtual ~NacosNamingMaintainService();
};
}//namespace nacos

#endif
