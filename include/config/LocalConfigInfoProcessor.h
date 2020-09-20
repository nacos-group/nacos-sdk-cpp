#ifndef __LOC_CFG_INF_PROCESSOR_H_
#define __LOC_CFG_INF_PROCESSOR_H_

#include "NacosString.h"
#include "NacosExceptions.h"

/**
 * Local Disaster Recovery Directory Tool
 *
 * @author Nacos
 */
class LocalConfigInfoProcessor {
public:
    static NacosString getFailover(const NacosString &serverName, const NacosString &dataId, const NacosString &group,
                                   const NacosString &tenant);

    /**
     * 获取本地缓存文件内容。NULL表示没有本地文件或抛出异常。
     */
    static NacosString getSnapshot(const NacosString &name, const NacosString &dataId, const NacosString &group,
                                   const NacosString &tenant);

    static NacosString readFile(const NacosString &file) throw(IOException);

    static void saveSnapshot(const NacosString &envName, const NacosString &dataId, const NacosString &group,
                             const NacosString &tenant, const NacosString &config);

    /**
     * 清除snapshot目录下所有缓存文件。
     */
    static void cleanAllSnapshot();

    static void cleanEnvSnapshot(const NacosString &envName);

    static NacosString
    getFailoverFile(const NacosString &serverName, const NacosString &dataId, const NacosString &group,
                    const NacosString &tenant);

    static NacosString getSnapshotFile(const NacosString &envName, const NacosString &dataId, const NacosString &group,
                                       const NacosString &tenant);

    static NacosString LOCAL_FILEROOT_PATH;
    static NacosString LOCAL_SNAPSHOT_PATH;

    static void init();
};

#endif