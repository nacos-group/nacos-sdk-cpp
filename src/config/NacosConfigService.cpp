#include "src/config/NacosConfigService.h"
#include "Constants.h"
#include "Parameters.h"
#include "utils/ParamUtils.h"
#include "Debug.h"

using namespace std;

namespace nacos{
NacosConfigService::NacosConfigService
        (
                AppConfigManager *appConfigManager,
                IHttpCli *httpCli,
                HttpDelegate *httpDelegate,
                ServerListManager *serverListManager,
                ClientWorker *clientWorker,
                LocalSnapshotManager *localSnapshotManager
        ) throw(NacosException) {
    _appConfigManager = appConfigManager;
    _httpCli = httpCli;
    _svrListMgr = serverListManager;
    _httpDelegate = httpDelegate;
    _clientWorker = clientWorker;
    _localSnapshotManager = localSnapshotManager;
}

NacosConfigService::~NacosConfigService() {
    log_debug("NacosConfigService::~NacosConfigService()\n");
    if (_clientWorker != NULL) {
        _clientWorker->stopListening();
        delete _clientWorker;
        _clientWorker = NULL;
    }

    if (_httpDelegate != NULL) {
        delete _httpDelegate;
        _httpDelegate = NULL;
    }

    if (_svrListMgr != NULL) {
        delete _svrListMgr;
        _svrListMgr = NULL;
    }

    if (_httpCli != NULL) {
        delete _httpCli;
        _httpCli = NULL;
    }

    if (_appConfigManager != NULL) {
        delete _appConfigManager;
        _appConfigManager = NULL;
    }
}

NacosString NacosConfigService::getConfig
        (
                const NacosString &dataId,
                const NacosString &group,
                long timeoutMs
        ) throw(NacosException) {
    return getConfigInner(getNamespace(), dataId, group, timeoutMs);
}

bool NacosConfigService::publishConfig
        (
                const NacosString &dataId,
                const NacosString &group,
                const NacosString &content
        ) throw(NacosException) {
    return publishConfigInner(getNamespace(), dataId, group, NULLSTR, NULLSTR, NULLSTR, content);
}

bool NacosConfigService::removeConfig
        (
                const NacosString &dataId,
                const NacosString &group
        ) throw(NacosException) {
    return removeConfigInner(getNamespace(), dataId, group, NULLSTR);
}

NacosString NacosConfigService::getConfigInner
        (
                const NacosString &tenant,
                const NacosString &dataId,
                const NacosString &group,
                long timeoutMs
        ) throw(NacosException) {
    NacosString result = NULLSTR;

    NacosString clientName = _appConfigManager->get(PropertyKeyConst::CLIENT_NAME);
    result = _localSnapshotManager->getFailover(clientName.c_str(), dataId, group, tenant);
    if (!NacosStringOps::isNullStr(result)) {
        log_warn("[%s] [get-config] get failover ok, dataId=%s, group=%s, tenant=%s, config=%s",
                 clientName.c_str(),
                 dataId.c_str(),
                 group.c_str(),
                 tenant.c_str(),
                 result.c_str());
        return result;
    }

    try {
        result = _clientWorker->getServerConfig(tenant, dataId, group, timeoutMs);
    } catch (NacosException &e) {
        if (e.errorcode() == NacosException::NO_RIGHT) {
            throw e;
        }

        result = _localSnapshotManager->getSnapshot(_appConfigManager->get(PropertyKeyConst::CLIENT_NAME), dataId, group, tenant);
    }
    return result;
}

bool NacosConfigService::removeConfigInner
        (
                const NacosString &tenant,
                const NacosString &dataId,
                const NacosString &group,
                const NacosString &tag
        ) throw(NacosException) {
    std::list <NacosString> headers;
    std::list <NacosString> paramValues;
    //Get the request url
    NacosString path = DEFAULT_CONTEXT_PATH + Constants::CONFIG_CONTROLLER_PATH;

    HttpResult res;

    paramValues.push_back("dataId");
    paramValues.push_back(dataId);

    NacosString parmGroupid = ParamUtils::null2defaultGroup(group);
    paramValues.push_back("group");
    paramValues.push_back(parmGroupid);

    if (!isNull(tenant)) {
        paramValues.push_back("tenant");
        paramValues.push_back(tenant);
    }

    NacosString serverAddr = _svrListMgr->getCurrentServerAddr();
    NacosString url = serverAddr + "/" + path;
    log_debug("httpDelete Assembled URL:%s\n", url.c_str());

    try {
        res = _httpDelegate->httpDelete(url, headers, paramValues, _httpDelegate->getEncode(), POST_TIMEOUT);
    }
    catch (NetworkException e) {
        log_warn("[remove] error, %s, %s, %s, msg: %s\n", dataId.c_str(), group.c_str(), tenant.c_str(), e.what());
        return false;
    }

    //If the server returns true, then this call succeeds
    if (res.content.compare("true") == 0) {
        return true;
    } else {
        return false;
    }
}

bool NacosConfigService::publishConfigInner
        (
                const NacosString &tenant,
                const NacosString &dataId,
                const NacosString &group,
                const NacosString &tag,
                const NacosString &appName,
                const NacosString &betaIps,
                const NacosString &content
        ) throw(NacosException) {
    //TODO:More stringent check, need to improve checkParam() function
    ParamUtils::checkParam(dataId, group, content);

    std::list <NacosString> headers;
    std::list <NacosString> paramValues;
    NacosString parmGroupid;
    //Get the request url
    NacosString path = DEFAULT_CONTEXT_PATH + Constants::CONFIG_CONTROLLER_PATH;

    HttpResult res;

    parmGroupid = ParamUtils::null2defaultGroup(group);
    ParamUtils::addKV(paramValues, "group", parmGroupid);

    ParamUtils::addKV(paramValues, "dataId", dataId);

    ParamUtils::addKV(paramValues, "content", content);

    if (!isNull(tenant)) {
        ParamUtils::addKV(paramValues, "tenant", tenant);
    }

    if (!isNull(appName)) {
        ParamUtils::addKV(paramValues, "appName", appName);
    }

    if (!isNull(tag)) {
        ParamUtils::addKV(paramValues, "tag", tag);
    }

    if (!isNull(betaIps)) {
        ParamUtils::addKV(paramValues, "betaIps", betaIps);
    }

    NacosString serverAddr = _svrListMgr->getCurrentServerAddr();
    NacosString url = serverAddr + "/" + path;
    log_debug("httpPost Assembled URL:%s\n", url.c_str());

    try {
        res = _httpDelegate->httpPost(url, headers, paramValues, _httpDelegate->getEncode(), POST_TIMEOUT);
    }
    catch (NetworkException e) {
        //
        log_warn("[NacosConfigService] [publish-single] exception, dataId=%s, group=%s, msg=%s\n", dataId.c_str(), group.c_str(),
                 tenant.c_str(), e.what());
        return false;
    }

    //If the server returns true, then this call succeeds
    if (res.content.compare("true") == 0) {
        return true;
    } else {
        return false;
    }
}

void NacosConfigService::addListener
        (
                const NacosString &dataId,
                const NacosString &group,
                Listener *listener
        ) throw(NacosException) {
    NacosString parmgroup = Constants::DEFAULT_GROUP;
    if (!isNull(group)) {
        parmgroup = group;
    }

    //TODO:give a constant to this hard-coded number
    NacosString cfgcontent;
    try {
        cfgcontent = getConfig(dataId, group, 3000);
    } catch (NacosException &e) {
        cfgcontent = "";
    }

    _clientWorker->addListener(dataId, parmgroup, getNamespace(), cfgcontent, listener);
    _clientWorker->startListening();
}

void NacosConfigService::removeListener
        (
                const NacosString &dataId,
                const NacosString &group,
                Listener *listener
        ) {
    NacosString parmgroup = Constants::DEFAULT_GROUP;
    if (!isNull(group)) {
        parmgroup = group;
    }
    log_debug("NacosConfigService::removeListener()\n");
    _clientWorker->removeListener(dataId, parmgroup, getNamespace(), listener);
}

}//namespace nacos
