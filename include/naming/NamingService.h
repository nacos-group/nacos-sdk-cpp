#ifndef __NAM_SVC_H_
#define __NAM_SVC_H_

#include <list>
#include "naming/Instance.h"
#include "NacosString.h"
#include "NacosExceptions.h"

class NamingService {
public:
    /**
     * register a instance to service
     *
     * @param serviceName name of service
     * @param ip          instance ip
     * @param port        instance port
     * @throw (NacosException) = 0
     */
    virtual void
    registerInstance(const NacosString &serviceName, const NacosString &ip, int port) throw(NacosException) = 0;

    /**
     * register a instance to service
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param ip          instance ip
     * @param port        instance port
     * @throw (NacosException) = 0
     */
    virtual void registerInstance(const NacosString &serviceName, const NacosString &groupName, const NacosString &ip,
                                  int port) throw(NacosException) = 0;

    /**
     * register a instance to service with specified cluster name
     *
     * @param serviceName name of service
     * @param ip          instance ip
     * @param port        instance port
     * @param clusterName instance cluster name
     * @throw (NacosException) = 0
     */
    virtual void registerInstance(const NacosString &serviceName, const NacosString &ip, int port,
                                  const NacosString &clusterName) throw(NacosException) = 0;

    /**
     * register a instance to service with specified cluster name
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param ip          instance ip
     * @param port        instance port
     * @param clusterName instance cluster name
     * @throw (NacosException) = 0
     */
    virtual void
    registerInstance(const NacosString &serviceName, const NacosString &groupName, const NacosString &ip, int port,
                     const NacosString &clusterName) throw(NacosException) = 0;

    /**
     * register a instance to service with specified instance properties
     *
     * @param serviceName name of service
     * @param instance    instance to register
     * @throw (NacosException) = 0
     */
    virtual void registerInstance(const NacosString &serviceName, Instance &instance) throw(NacosException) = 0;

    /**
     * register a instance to service with specified instance properties
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param instance    instance to register
     * @throw (NacosException) = 0
     */
    virtual void registerInstance(const NacosString &serviceName, const NacosString &groupName,
                                  Instance &instance) throw(NacosException) = 0;

    /**
     * deregister instance from a service
     *
     * @param serviceName name of service
     * @param ip          instance ip
     * @param port        instance port
     * @throw (NacosException) = 0
     */
    virtual void
    deregisterInstance(const NacosString &serviceName, const NacosString &ip, int port) throw(NacosException) = 0;

    /**
     * deregister instance from a service
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param ip          instance ip
     * @param port        instance port
     * @throw (NacosException) = 0
     */
    virtual void deregisterInstance(const NacosString &serviceName, const NacosString &groupName, const NacosString &ip,
                                    int port) throw(NacosException) = 0;

    /**
     * deregister instance with specified cluster name from a service
     *
     * @param serviceName name of service
     * @param ip          instance ip
     * @param port        instance port
     * @param clusterName instance cluster name
     * @throw (NacosException) = 0
     */
    virtual void deregisterInstance(const NacosString &serviceName, const NacosString &ip, int port,
                                    const NacosString &clusterName) throw(NacosException) = 0;

    /**
     * deregister instance with specified cluster name from a service
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param ip          instance ip
     * @param port        instance port
     * @param clusterName instance cluster name
     * @throw (NacosException) = 0
     */
    virtual void
    deregisterInstance(const NacosString &serviceName, const NacosString &groupName, const NacosString &ip, int port,
                       const NacosString &clusterName) throw(NacosException) = 0;

    /**
     * deregister instance with full instance information
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param instance    instance information
     * @throw (NacosException) = 0
     */
    virtual void deregisterInstance(const NacosString &serviceName, const NacosString &groupName,
                                    Instance &instance) throw(NacosException) = 0;

    /**
     * get all instances of a service
     *
     * @param serviceName name of service
     * @return A list of instance
     * @throw (NacosException) = 0
     */
    virtual std::list <Instance> getAllInstances(const NacosString &serviceName) throw(NacosException) = 0;

    /**
     * get all instances of a service
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @return A list of instance
     * @throw (NacosException) = 0
     */
    virtual std::list <Instance>
    getAllInstances(const NacosString &serviceName, const NacosString &groupName) throw(NacosException) = 0;

    /**
     * Get all instances within specified clusters of a service
     *
     * @param serviceName name of service
     * @param clusters    list of cluster
     * @return A list of qualified instance
     * @throw (NacosException) = 0
     */
    virtual std::list <Instance>
    getAllInstances(const NacosString &serviceName, std::list <NacosString> clusters) throw(NacosException) = 0;

    /**
     * Get all instances within specified clusters of a service
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param clusters    list of cluster
     * @return A list of qualified instance
     * @throw (NacosException) = 0
     */
    virtual std::list <Instance> getAllInstances(const NacosString &serviceName, const NacosString &groupName,
                                                 std::list <NacosString> clusters) throw(NacosException) = 0;

    /**
     * Get qualified instances of service
     *
     * @param serviceName name of service
     * @param healthy     a flag to indicate returning healthy or unhealthy instances
     * @return A qualified list of instance
     * @throw (NacosException) = 0
     */
    //virtual std::list<Instance> selectInstances(const NacosString &serviceName, bool healthy) throw (NacosException) = 0;

    /**
     * Get qualified instances of service
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param healthy     a flag to indicate returning healthy or unhealthy instances
     * @return A qualified list of instance
     * @throw (NacosException) = 0
     */
    //virtual std::list<Instance> selectInstances(const NacosString &serviceName, const NacosString &groupName, bool healthy) throw (NacosException) = 0;

    /**
     * Get qualified instances of service
     *
     * @param serviceName name of service
     * @param healthy     a flag to indicate returning healthy or unhealthy instances
     * @param subscribe   if subscribe the service
     * @return A qualified list of instance
     * @throw (NacosException) = 0
     */
    //virtual std::list<Instance> selectInstances(const NacosString &serviceName, bool healthy, bool subscribe) throw (NacosException) = 0;

    /**
     * Get qualified instances of service
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param healthy     a flag to indicate returning healthy or unhealthy instances
     * @param subscribe   if subscribe the service
     * @return A qualified list of instance
     * @throw (NacosException) = 0
     */
    //virtual std::list<Instance> selectInstances(const NacosString &serviceName, const NacosString &groupName, bool healthy, bool subscribe) throw (NacosException) = 0;

    /**
     * Get qualified instances within specified clusters of service
     *
     * @param serviceName name of service
     * @param clusters    list of cluster
     * @param healthy     a flag to indicate returning healthy or unhealthy instances
     * @return A qualified list of instance
     * @throw (NacosException) = 0
     */
    //virtual std::list<Instance> selectInstances(const NacosString &serviceName, std::list<NacosString> clusters, bool healthy) throw (NacosException) = 0;

    /**
     * Get qualified instances within specified clusters of service
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param clusters    list of cluster
     * @param healthy     a flag to indicate returning healthy or unhealthy instances
     * @return A qualified list of instance
     * @throw (NacosException) = 0
     */
    //virtual std::list<Instance> selectInstances(const NacosString &serviceName, const NacosString &groupName, std::list<NacosString> clusters, bool healthy) throw (NacosException) = 0;

    /**
     * Get qualified instances within specified clusters of service
     *
     * @param serviceName name of service
     * @param clusters    list of cluster
     * @param healthy     a flag to indicate returning healthy or unhealthy instances
     * @param subscribe   if subscribe the service
     * @return A qualified list of instance
     * @throw (NacosException) = 0
     */
    //virtual std::list<Instance> selectInstances(const NacosString &serviceName, std::list<NacosString> clusters, bool healthy, bool subscribe) throw (NacosException) = 0;

    /**
     * Get qualified instances within specified clusters of service
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param clusters    list of cluster
     * @param healthy     a flag to indicate returning healthy or unhealthy instances
     * @param subscribe   if subscribe the service
     * @return A qualified list of instance
     * @throw (NacosException) = 0
     */
    //virtual std::list<Instance> selectInstances(const NacosString &serviceName, const NacosString &groupName, std::list<NacosString> clusters, bool healthy, bool subscribe) throw (NacosException) = 0;

    /**
     * Select one healthy instance of service using predefined load balance strategy
     *
     * @param serviceName name of service
     * @return qualified instance
     * @throw (NacosException) = 0
     */
    //virtual Instance selectOneHealthyInstance(const NacosString &serviceName) throw (NacosException) = 0;

    /**
     * Select one healthy instance of service using predefined load balance strategy
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @return qualified instance
     * @throw (NacosException) = 0
     */
    //virtual Instance selectOneHealthyInstance(const NacosString &serviceName, const NacosString &groupName) throw (NacosException) = 0;

    /**
     * select one healthy instance of service using predefined load balance strategy
     *
     * @param serviceName name of service
     * @param subscribe   if subscribe the service
     * @return qualified instance
     * @throw (NacosException) = 0
     */
    //virtual Instance selectOneHealthyInstance(const NacosString &serviceName, bool subscribe) throw (NacosException) = 0;

    /**
     * select one healthy instance of service using predefined load balance strategy
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param subscribe   if subscribe the service
     * @return qualified instance
     * @throw (NacosException) = 0
     */
    //virtual Instance selectOneHealthyInstance(const NacosString &serviceName, const NacosString &groupName, bool subscribe) throw (NacosException) = 0;

    /**
     * Select one healthy instance of service using predefined load balance strategy
     *
     * @param serviceName name of service
     * @param clusters    a list of clusters should the instance belongs to
     * @return qualified instance
     * @throw (NacosException) = 0
     */
    //virtual Instance selectOneHealthyInstance(const NacosString &serviceName, std::list<NacosString> clusters) throw (NacosException) = 0;

    /**
     * Select one healthy instance of service using predefined load balance strategy
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param clusters    a list of clusters should the instance belongs to
     * @return qualified instance
     * @throw (NacosException) = 0
     */
    //virtual Instance selectOneHealthyInstance(const NacosString &serviceName, const NacosString &groupName, std::list<NacosString> clusters) throw (NacosException) = 0;

    /**
     * Select one healthy instance of service using predefined load balance strategy
     *
     * @param serviceName name of service
     * @param clusters    a list of clusters should the instance belongs to
     * @param subscribe   if subscribe the service
     * @return qualified instance
     * @throw (NacosException) = 0
     */
    //virtual Instance selectOneHealthyInstance(const NacosString &serviceName, std::list<NacosString> clusters, bool subscribe) throw (NacosException) = 0;

    /**
     * Select one healthy instance of service using predefined load balance strategy
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param clusters    a list of clusters should the instance belongs to
     * @param subscribe   if subscribe the service
     * @return qualified instance
     * @throw (NacosException) = 0
     */
    //virtual Instance selectOneHealthyInstance(const NacosString &serviceName, const NacosString &groupName, std::list<NacosString> clusters, bool subscribe) throw (NacosException) = 0;

    /**
     * Subscribe service to receive events of instances alteration
     *
     * @param serviceName name of service
     * @param listener    event listener
     * @throw (NacosException) = 0
     */
    //virtual void subscribe(const NacosString &serviceName, EventListener listener) throw (NacosException) = 0;

    /**
     * Subscribe service to receive events of instances alteration
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param listener    event listener
     * @throw (NacosException) = 0
     */
    //virtual void subscribe(const NacosString &serviceName, const NacosString &groupName, EventListener listener) throw (NacosException) = 0;

    /**
     * Subscribe service to receive events of instances alteration
     *
     * @param serviceName name of service
     * @param clusters    list of cluster
     * @param listener    event listener
     * @throw (NacosException) = 0
     */
    //virtual void subscribe(const NacosString &serviceName, std::list<NacosString> clusters, EventListener listener) throw (NacosException) = 0;

    /**
     * Subscribe service to receive events of instances alteration
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param clusters    list of cluster
     * @param listener    event listener
     * @throw (NacosException) = 0
     */
    //virtual void subscribe(const NacosString &serviceName, const NacosString &groupName, std::list<NacosString> clusters, EventListener listener) throw (NacosException) = 0;

    /**
     * Unsubscribe event listener of service
     *
     * @param serviceName name of service
     * @param listener    event listener
     * @throw (NacosException) = 0
     */
    //virtual void unsubscribe(const NacosString &serviceName, EventListener listener) throw (NacosException) = 0;

    /**
     * unsubscribe event listener of service
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param listener    event listener
     * @throw (NacosException) = 0
     */
    //virtual void unsubscribe(const NacosString &serviceName, const NacosString &groupName, EventListener listener) throw (NacosException) = 0;

    /**
     * Unsubscribe event listener of service
     *
     * @param serviceName name of service
     * @param clusters    list of cluster
     * @param listener    event listener
     * @throw (NacosException) = 0
     */
    //virtual void unsubscribe(const NacosString &serviceName, std::list<NacosString> clusters, EventListener listener) throw (NacosException) = 0;

    /**
     * Unsubscribe event listener of service
     *
     * @param serviceName name of service
     * @param groupName   group of service
     * @param clusters    list of cluster
     * @param listener    event listener
     * @throw (NacosException) = 0
     */
    //virtual void unsubscribe(const NacosString &serviceName, const NacosString &groupName, std::list<NacosString> clusters, EventListener listener) throw (NacosException) = 0;

    /**
     * Get all service names from server
     *
     * @param pageNo   page index
     * @param pageSize page size
     * @return list of service names
     * @throw (NacosException) = 0
     */
    //virtual ListView<NacosString> getServicesOfServer(int pageNo, int pageSize) throw (NacosException) = 0;

    /**
     * Get all service names from server
     *
     * @param pageNo    page index
     * @param pageSize  page size
     * @param groupName group name
     * @return list of service names
     * @throw (NacosException) = 0
     */
    //virtual ListView<NacosString> getServicesOfServer(int pageNo, int pageSize, const NacosString &groupName) throw (NacosException) = 0;

    /**
     * Get all service names from server with selector
     *
     * @param pageNo   page index
     * @param pageSize page size
     * @param selector selector to filter the resource
     * @return list of service names
     * @throw (NacosException) = 0
     * @since 0.7.0
     */
    //virtual ListView<NacosString> getServicesOfServer(int pageNo, int pageSize, AbstractSelector selector) throw (NacosException) = 0;

    /**
     * Get all service names from server with selector
     *
     * @param pageNo    page index
     * @param pageSize  page size
     * @param groupName group name
     * @param selector  selector to filter the resource
     * @return list of service names
     * @throw (NacosException) = 0
     */
    //virtual ListView<NacosString> getServicesOfServer(int pageNo, int pageSize, const NacosString &groupName, AbstractSelector selector) throw (NacosException) = 0;

    /**
     * Get all subscribed services of current client
     *
     * @return subscribed services
     * @throw (NacosException) = 0
     */
    //virtual std::list<ServiceInfo> getSubscribeServices() throw (NacosException) = 0;

    /**
     * get server health status
     *
     * @return is server healthy
     */
    //virtual NacosString getServerStatus() = 0;

    virtual ~NamingService() {};
};

#endif