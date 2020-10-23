#include "src/config/NacosConfigService.h"
#include "Constants.h"
#include "Parameters.h"
#include "utils/ParamUtils.h"
#include "Debug.h"
#include "src/md5/md5.h"

using namespace std;

namespace nacos{
NacosConfigService::NacosConfigService
        (
                AppConfigManager *_appConfigManager,
                IHttpCli *httpCli,
                HttpDelegate *httpDelegate,
                ServerListManager *_serverListManager,
                ClientWorker *_clientWorker
        ) throw(NacosException) {
    appConfigManager = _appConfigManager;
    _httpCli = httpCli;
    svrListMgr = _serverListManager;
    _httpDelegate = httpDelegate;
    clientWorker = _clientWorker;
}

NacosConfigService::~NacosConfigService() {
    log_debug("NacosConfigService::~NacosConfigService()\n");
    if (clientWorker != NULL) {
        clientWorker->stopListening();
        delete clientWorker;
        clientWorker = NULL;
    }

    if (_httpDelegate != NULL) {
        delete _httpDelegate;
        _httpDelegate = NULL;
    }

    if (svrListMgr != NULL) {
        delete svrListMgr;
        svrListMgr = NULL;
    }

    if (_httpCli != NULL) {
        delete _httpCli;
        _httpCli = NULL;
    }

    if (appConfigManager != NULL) {
        delete appConfigManager;
        appConfigManager = NULL;
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
    return clientWorker->getServerConfig(tenant, dataId, group, timeoutMs);
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

    NacosString serverAddr = svrListMgr->getCurrentServerAddr();
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

    NacosString serverAddr = svrListMgr->getCurrentServerAddr();
    NacosString url = serverAddr + "/" + path;
    log_debug("httpPost Assembled URL:%s\n", url.c_str());

    try {
        res = _httpDelegate->httpPost(url, headers, paramValues, _httpDelegate->getEncode(), POST_TIMEOUT);
    }
    catch (NetworkException e) {
        //
        log_warn("[{}] [publish-single] exception, dataId=%s, group=%s, msg=%s\n", dataId.c_str(), group.c_str(),
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
    NacosString cfgcontent = getConfig(dataId, group, 3000);

    clientWorker->addListener(dataId, parmgroup, getNamespace(), cfgcontent, listener);
    clientWorker->startListening();
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
    clientWorker->removeListener(dataId, parmgroup, getNamespace(), listener);
}

}//namespace nacos
