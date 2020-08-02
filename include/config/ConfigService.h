#ifndef __CFG_SVC_H_
#define __CFG_SVC_H_
#include "NacosExceptions.h"
#include "NacosString.h"
#include "listen/Listener.h"

class ConfigService
{
public:
	/**
	 * Get config
	 *
	 * @param dataId    dataId
	 * @param group     group
	 * @param timeoutMs read timeout
	 * @return config value
	 * @throws NacosException NacosException
	 */
	virtual NacosString getConfig(const NacosString &dataId, const NacosString &group, long timeoutMs) throw(NacosException) = 0;

	/**
	 * Add a listener to the configuration, after the server modified the
	 * configuration, the client will use the incoming listener callback.
	 * Recommended asynchronous processing, the application can implement the
	 * getExecutor method in the ManagerListener, provide a thread pool of
	 * execution. If provided, use the main thread callback, May block other
	 * configurations or be blocked by other configurations.
	 *
	 * @param dataId   dataId
	 * @param group    group
	 * @param listener listener
	 * @throws NacosException NacosException
	 */
	virtual void addListener(const NacosString &dataId, const NacosString &group, Listener *listener) throw(NacosException) = 0;

	/**
	 * Publish config.
	 *
	 * @param dataId  dataId
	 * @param group   group
	 * @param content content
	 * @return Whether publish
	 * @throws NacosException NacosException
	 */
	virtual bool publishConfig(const NacosString &dataId, const NacosString &group, const NacosString &content) throw(NacosException) = 0;

	/**
	 * Remove config
	 *
	 * @param dataId dataId
	 * @param group  group
	 * @return whether remove
	 * @throws NacosException NacosException
	 */
	virtual bool removeConfig(const NacosString &dataId, const NacosString &group) throw(NacosException) = 0;

	/**
	 * Remove listener
	 *
	 * @param dataId   dataId
	 * @param group    group
	 * @param listener listener
	 */
	//virtual void removeListener(NacosString dataId, NacosString group, Listener listener) = 0;

	/**
	 * Get server status
	 *
	 * @return whether health
	 */
	//virtual NacosString getServerStatus() = 0;

	virtual ~ConfigService() {};
};

#endif