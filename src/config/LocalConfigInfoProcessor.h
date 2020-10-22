#ifndef __LOC_CFG_INF_PROCESSOR_H_
#define __LOC_CFG_INF_PROCESSOR_H_

#include "NacosString.h"
#include "NacosExceptions.h"
#include "src/config/AppConfigManager.h"

/**
 * Local Disaster Recovery Directory Tool
 *
 * @author Nacos
 */

namespace nacos{
class LocalConfigInfoProcessor {
private:
    AppConfigManager *_appCfgMgr;
    NacosString LOCAL_SNAPSHOT_PATH;
public:

    LocalConfigInfoProcessor(AppConfigManager *appConfigManager);

    NacosString getFailover(const NacosString &serverName, const NacosString &dataId, const NacosString &group,
                                   const NacosString &tenant);

    /**
     * Accuire local cache content, returns NULLSTR when the file does not exist or an exception is thrown
     * 获取本地缓存文件内容。NULL表示没有本地文件或抛出异常。
     */
    NacosString getSnapshot(const NacosString &name, const NacosString &dataId, const NacosString &group,
                                   const NacosString &tenant);

    NacosString readFile(const NacosString &file) throw(IOException);

    void saveSnapshot(const NacosString &envName, const NacosString &dataId, const NacosString &group,
                             const NacosString &tenant, const NacosString &config);

    /**
     * Purge all cached files in snapshot directory
     * 清除snapshot目录下所有缓存文件。
     */
    void cleanAllSnapshot();

    void cleanEnvSnapshot(const NacosString &envName);

    NacosString
    getFailoverFile(const NacosString &serverName, const NacosString &dataId, const NacosString &group,
                    const NacosString &tenant);

    NacosString getSnapshotFile(const NacosString &envName, const NacosString &dataId, const NacosString &group,
                                       const NacosString &tenant);
};
}//namespace nacos

#endif