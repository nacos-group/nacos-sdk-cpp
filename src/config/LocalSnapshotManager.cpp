#include "LocalSnapshotManager.h"
#include <stdio.h>
#include <list>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include "NacosExceptions.h"
#include "NacosString.h"
#include "src/utils/ParamUtils.h"
#include "src/utils/DirUtils.h"
#include "SnapShotSwitch.h"
#include "JVMUtil.h"
#include "ConcurrentDiskUtil.h"
#include "IOUtils.h"
#include "constant/ConfigConstant.h"
#include "constant/PropertyKeyConst.h"
#include "src/log/Logger.h"

namespace nacos{

NacosString LocalSnapshotManager::getFailover(const NacosString &serverName, const NacosString &dataId,
                                                  const NacosString &group, const NacosString &tenant) {
    NacosString localPath = getFailoverFile(serverName, dataId, group, tenant);

    if (IOUtils::checkNotExistOrNotFile(localPath)) {
        log_debug("[LocalSnapshotManager]-getFailover:[servername=%s] failover file %s doesn't exist\n",
                  serverName.c_str(), localPath.c_str());
        return NULLSTR;
    }

    try {
        return readFile(localPath);
    } catch (IOException &ioe) {
        log_error("[LocalSnapshotManager]-getFailover:[servername=%s] get failover error, file: %s, exception:%s\n", serverName.c_str(), localPath.c_str(), ioe.what());
        return NULLSTR;
    }
};

/**
 * 获取本地缓存文件内容。NULL表示没有本地文件或抛出异常。
 */
NacosString LocalSnapshotManager::getSnapshot
        (
                const NacosString &name,
                const NacosString &dataId,
                const NacosString &group,
                const NacosString &tenant
        ) {
    if (!SnapShotSwitch::getIsSnapShot()) {
        return NULLSTR;
    }
    NacosString file = getSnapshotFile(name, dataId, group, tenant);
    if (IOUtils::checkNotExistOrNotFile(file)) {
        return NULLSTR;
    }

    try {
        return readFile(file);
    } catch (IOException &ioe) {
        log_error("[LocalSnapshotManager]-getSnapshot:[servername=%s]+get snapshot error, file:%s what:%s\n", name.c_str(), file.c_str(), ioe.what());
        return NULLSTR;
    }
};

NacosString LocalSnapshotManager::readFile(const NacosString &file) NACOS_THROW(IOException) {
    if (IOUtils::checkNotExistOrNotFile(file)) {
        return NULLSTR;
    }

    if (JVMUtil::isMultiInstance()) {
        return ConcurrentDiskUtil::getFileContent(file, ConfigConstant::ENCODE);
    } else {
        return IOUtils::readStringFromFile(file, ConfigConstant::ENCODE);
    }
};

void
LocalSnapshotManager::saveSnapshot(const NacosString &envName, const NacosString &dataId, const NacosString &group,
                                       const NacosString &tenant, const NacosString &config) {
    if (!SnapShotSwitch::getIsSnapShot()) {
        return;
    }

    NacosString file = getSnapshotFile(envName, dataId, group, tenant);
    if (isNull(config)) {
        int remove_result = remove(file.c_str());
        if (remove_result)//error happens when removing the file
        {
            //usually we get this error because we are deleting a non-existent file
            log_debug("[LocalSnapshotManager]-saveSnapshot:[servername=%s] delete snapshot error, remove() returns %d, errno = %d\n",
                      envName.c_str(), remove_result, errno);
        }
    } else {
        NacosString parentFile = IOUtils::getParentFile(file);
        if (IOUtils::checkNotExistOrNotDir(parentFile)) {
            IOUtils::recursivelyCreate(parentFile);
            //LOGGER.error("[{}] save snapshot error", envName);
        }

        if (JVMUtil::isMultiInstance()) {
            ConcurrentDiskUtil::writeFileContent(file, config, ConfigConstant::ENCODE);
        } else {
            IOUtils::writeStringToFile(file, config, ConfigConstant::ENCODE);
        }

        //LOGGER.error("[" + envName + "] save snapshot error, " + file, ioe);
    }
};

/**
 * 清除snapshot目录下所有缓存文件。
 */
void LocalSnapshotManager::cleanAllSnapshot() {
    std::list <NacosString> rootFile = IOUtils::listFiles(LOCAL_SNAPSHOT_PATH);

    for (std::list<NacosString>::iterator it = rootFile.begin(); it != rootFile.end(); it++) {
        //endsWith("_nacos")
        if (it->length() >= 6 && (it->rfind("_nacos") == it->length() - 6)) {
            IOUtils::cleanDirectory(LOCAL_SNAPSHOT_PATH + "/" + *it);
        }
    }
    //LOGGER.error("clean all snapshot error, " + ioe.toString(), ioe);
};

void LocalSnapshotManager::cleanEnvSnapshot(const NacosString &envName) {
    NacosString tmp = LOCAL_SNAPSHOT_PATH + "/" + envName + "_nacos";
    tmp += "/snapshot";
    //I think we should remove -tenant also, so for one envname, cache for all tenants within the environment will be purged
    NacosString tmp_tenant = tmp + "-tenant";
    IOUtils::cleanDirectory(tmp);
    IOUtils::cleanDirectory(tmp_tenant);
    log_info("[LocalSnapshotManager]-cleanEnvSnapshot:success delete %s-snapshot: %s\n", envName.c_str(), tmp.c_str());
};

NacosString LocalSnapshotManager::getFailoverFile(const NacosString &serverName, const NacosString &dataId,
                                                      const NacosString &group, const NacosString &tenant) {
    NacosString Failoverfile = LOCAL_SNAPSHOT_PATH + "/" + serverName + "_nacos";
    Failoverfile += "/data";
    if (ParamUtils::isBlank(tenant)) {
        Failoverfile += "/config-data";
    } else {
        Failoverfile += "/config-data-tenant/";
        Failoverfile += tenant;
    }
    if (NacosStringOps::isNullStr(group)) {
        Failoverfile += "/" + ConfigConstant::DEFAULT_GROUP + "/" + dataId;
    } else {
        Failoverfile += "/" + group + "/" + dataId;
    }
    return Failoverfile;
};

NacosString LocalSnapshotManager::getSnapshotFile(const NacosString &envName, const NacosString &dataId,
                                                      const NacosString &group, const NacosString &tenant) {
    NacosString filename = LOCAL_SNAPSHOT_PATH + "/" + envName + "_nacos";
    if (isNull(tenant)) {
        filename += "/snapshot";
    } else {
        filename += "/snapshot-tenant/" + tenant;
    }

    if (NacosStringOps::isNullStr(group)) {
        filename += "/" + ConfigConstant::DEFAULT_GROUP + "/" + dataId;
    } else {
        filename += "/" + group + "/" + dataId;
    }
    return filename;
};

LocalSnapshotManager::LocalSnapshotManager(AppConfigManager *appConfigManager) {
    this->_appCfgMgr = appConfigManager;
    LOCAL_SNAPSHOT_PATH = _appCfgMgr->get(PropertyKeyConst::NACOS_SNAPSHOT_PATH);
    LOCAL_FAILOVER_PATH = _appCfgMgr->get(PropertyKeyConst::NACOS_SNAPSHOT_PATH);

    log_debug("LocalConfigInfoProcessor::LocalConfigInfoProcessor() LOCAL_SNAPSHOT_PATH = %s\n", LocalSnapshotManager::LOCAL_SNAPSHOT_PATH.c_str());
}
}//namespace nacos