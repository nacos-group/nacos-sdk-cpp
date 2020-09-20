#include <map>
#include <stdlib.h>
#include "naming/NamingProxy.h"
#include "naming/NamingCommonParams.h"
#include "utils/ParamUtils.h"
#include "utils/UtilAndComs.h"
#include "utils/UuidUtils.h"
#include "utils/NetUtils.h"
#include "json/JSON.h"
#include "http/httpStatCode.h"
#include "Debug.h"
#include "NacosExceptions.h"

using namespace std;

NamingProxy::NamingProxy(HTTPCli *httpcli, ServerListManager *_serverListManager, AppConfigManager *_appConfigManager) {
    log_debug("NamingProxy Constructor:\n"
              "namespace:%s, endpoint:%s, Servers:%s\n",
              _serverListManager->getNamespace().c_str(), _serverListManager->getEndpoint().c_str(),
              _serverListManager->toString().c_str());
    serverListManager = _serverListManager;
    httpCli = httpcli;
    appConfigManager = _appConfigManager;
    serverPort = "8848";
    _http_req_timeout = atoi(appConfigManager->get(PropertyKeyConst::HTTP_REQ_TIMEOUT).c_str());

    if (serverListManager->getServerCount() == 1) {
        nacosDomain = serverListManager->getServerList().begin()->getCompleteAddress();
    }
    log_debug("The serverlist:%s\n", _serverListManager->toString().c_str());
}

NamingProxy::~NamingProxy() {
    httpCli = NULL;

    if (appConfigManager != NULL) {
        delete appConfigManager;
        appConfigManager = NULL;
    }

    if (serverListManager != NULL) {
        delete serverListManager;
        serverListManager = NULL;
    }
}

void NamingProxy::registerService(const NacosString &serviceName, const NacosString &groupName,
                                  Instance &instance) throw(NacosException) {
    log_info("[REGISTER-SERVICE] %s registering service %s with instance: %s\n",
             getNamespaceId().c_str(), serviceName.c_str(), instance.toString().c_str());

    map <NacosString, NacosString> params;
    params[NamingCommonParams::NAMESPACE_ID] = getNamespaceId();
    params[NamingCommonParams::SERVICE_NAME] = serviceName;
    params[NamingCommonParams::GROUP_NAME] = groupName;
    params[NamingCommonParams::CLUSTER_NAME] = instance.clusterName;
    params["ip"] = instance.ip;
    params["port"] = NacosStringOps::valueOf(instance.port);
    params["weight"] = NacosStringOps::valueOf(instance.weight);
    params["enable"] = NacosStringOps::valueOf(instance.enabled);
    params["healthy"] = NacosStringOps::valueOf(instance.healthy);
    params["ephemeral"] = NacosStringOps::valueOf(instance.ephemeral);
    //TODO:transfer metadata in JSON form
    params["metadata"] = JSON::toJSONString(instance.metadata);

    reqAPI(UtilAndComs::NACOS_URL_INSTANCE, params, HTTPCli::POST);
}

void NamingProxy::deregisterService(const NacosString &serviceName, Instance &instance) throw(NacosException) {
    log_info("[DEREGISTER-SERVICE] %s deregistering service %s with instance: %s\n",
             getNamespaceId().c_str(), serviceName.c_str(), instance.toString().c_str());

    map <NacosString, NacosString> params;
    params[NamingCommonParams::NAMESPACE_ID] = getNamespaceId();
    params[NamingCommonParams::SERVICE_NAME] = serviceName;
    params[NamingCommonParams::CLUSTER_NAME] = instance.clusterName;
    params["ip"] = instance.ip;
    params["port"] = NacosStringOps::valueOf(instance.port);
    params["ephemeral"] = NacosStringOps::valueOf(instance.ephemeral);

    reqAPI(UtilAndComs::NACOS_URL_INSTANCE, params, HTTPCli::DELETE);
}

NacosString NamingProxy::queryList(const NacosString &serviceName, const NacosString &clusters, int udpPort,
                                   bool healthyOnly) throw(NacosException) {
    map <NacosString, NacosString> params;
    params[NamingCommonParams::NAMESPACE_ID] = getNamespaceId();
    params[NamingCommonParams::SERVICE_NAME] = serviceName;
    params["clusters"] = clusters;
    params["udpPort"] = NacosStringOps::valueOf(udpPort);
    params["clientIP"] = NetUtils::localIP();
    params["healthyOnly"] = NacosStringOps::valueOf(healthyOnly);

    return reqAPI(UtilAndComs::NACOS_URL_BASE + "/instance/list", params, HTTPCli::GET);
}

NacosString
NamingProxy::reqAPI(const NacosString &api, map <NacosString, NacosString> &params, int method) throw(NacosException) {
    params[NamingCommonParams::NAMESPACE_ID] = getNamespaceId();
    list <NacosServerInfo> servers = serverListManager->getServerList();

    if (serverListManager->getServerCount() == 0) {
        throw NacosException(0, "no server available");
    }

    NacosString errmsg;
    if (!servers.empty()) {
        size_t maxSvrSlot = servers.size();
        log_debug("nr_servers:%d\n", maxSvrSlot);
        srand(time(NULL));
        size_t selectedServer = rand() % maxSvrSlot;
        log_debug("selected_server:%d\n", selectedServer);

        for (size_t i = 0; i < servers.size(); i++) {
            NacosServerInfo server = ParamUtils::getNthElem(servers, selectedServer);
            log_debug("Trying to access server:%s\n", server.toString().c_str());
            try {
                return callServer(api, params, server.getCompleteAddress(), method);
            }
            catch (NacosException &e) {
                errmsg = e.what();
                log_error("request %s failed.\n", server.toString().c_str());
            }
            catch (exception &e) {
                errmsg = e.what();
                log_error("request %s failed.\n", server.toString().c_str());
            }

            selectedServer = (selectedServer + 1) % servers.size();
        }

        throw NacosException(0, "failed to req API:" + api + " after all servers(" + serverListManager->toString() +
                                ") tried: "
                                + errmsg);
    }

    for (int i = 0; i < UtilAndComs::REQUEST_DOMAIN_RETRY_COUNT; i++) {
        try {
            return callServer(api, params, nacosDomain);
        }
        catch (exception &e) {
            errmsg = e.what();
            log_error("[NA] req api:%s failed, server(%s), e = %s\n", api.c_str(), nacosDomain.c_str(), e.what());
        }
    }

    throw NacosException(0, "failed to req API:/api/" + api + " after all servers(" + serverListManager->toString() +
                            ") tried: " + errmsg);
}

NacosString NamingProxy::callServer
        (
                const NacosString &api,
                map <NacosString, NacosString> &params,
                const NacosString &curServer
        ) throw(NacosException) {
    return callServer(api, params, nacosDomain, HTTPCli::GET);
}

NacosString NamingProxy::callServer
        (
                const NacosString &api,
                map <NacosString, NacosString> &params,
                const NacosString &curServer,
                int method
        ) throw(NacosException) {
    NacosString requestUrl;
    //Current server address doesn't have SERVER_ADDR_IP_SPLITER, which means
    if (!ParamUtils::contains(curServer, UtilAndComs::SERVER_ADDR_IP_SPLITER)) {
        requestUrl = curServer + UtilAndComs::SERVER_ADDR_IP_SPLITER + serverPort;
    } else {
        requestUrl = curServer;
    }

    requestUrl = HTTPCli::getPrefix() + requestUrl + api;

    HttpResult requestRes;
    list <NacosString> headers;
    headers = builderHeaders();

    try {
        switch (method) {
            case HTTPCli::GET:
                requestRes = httpCli->httpGet(requestUrl, headers, params, UtilAndComs::ENCODING,
                                              _http_req_timeout);
                break;
            case HTTPCli::PUT:
                requestRes = httpCli->httpPut(requestUrl, headers, params, UtilAndComs::ENCODING,
                                              _http_req_timeout);
                break;
            case HTTPCli::POST:
                requestRes = httpCli->httpPost(requestUrl, headers, params, UtilAndComs::ENCODING,
                                               _http_req_timeout);
                break;
            case HTTPCli::DELETE:
                requestRes = httpCli->httpDelete(requestUrl, headers, params, UtilAndComs::ENCODING,
                                                 _http_req_timeout);
                break;
        }
    }
    catch (NetworkException &e) {
        NacosString errMsg = "Failed to request server, ";
        errMsg += e.what();
        throw NacosException(NacosException::SERVER_ERROR, errMsg);
    }

    if (requestRes.code == HTTP_OK) {
        return requestRes.content;
    }

    if (requestRes.code == HTTP_NOT_MODIFIED) {
        return NULLSTR;
    }
    //TODO:Metrics & Monitoring

    throw NacosException(NacosException::SERVER_ERROR,
                         "failed to req API:" + requestUrl + " code:" + NacosStringOps::valueOf(requestRes.code) +
                         " errormsg:" + requestRes.content);
}

inline NacosString NamingProxy::getNamespaceId() {
    return appConfigManager->get(PropertyKeyConst::NAMESPACE);
}

list <NacosString> NamingProxy::builderHeaders() {
    list <NacosString> headers;
    headers.push_back("Client-Version");
    headers.push_back(UtilAndComs::VERSION);

    headers.push_back("User-Agent");
    headers.push_back(UtilAndComs::VERSION);

    headers.push_back("Accept-Encoding");
    headers.push_back("gzip,deflate,sdch");

    headers.push_back("Connection");
    headers.push_back("Keep-Alive");

    headers.push_back("RequestId");
    headers.push_back(UuidUtils::generateUuid());

    headers.push_back("Request-Module");
    headers.push_back("Naming");
    return headers;
}

long NamingProxy::sendBeat(BeatInfo &beatInfo) {
    try {
        NacosString beatInfoStr = beatInfo.toString();
        log_info("[BEAT] %s sending beat to server: %s\n", getNamespaceId().c_str(), beatInfoStr.c_str());
        map <NacosString, NacosString> params;
        params[NamingCommonParams::BEAT] = JSON::toJSONString(beatInfo);
        params[NamingCommonParams::NAMESPACE_ID] = getNamespaceId();
        params[NamingCommonParams::SERVICE_NAME] = beatInfo.serviceName;
        NacosString result = reqAPI(UtilAndComs::NACOS_URL_BASE + "/instance/beat", params, HTTPCli::PUT);
        //JSONObject jsonObject = JSON.parseObject(result);

        if (!isNull(result)) {
            return JSON::getLong(result, "clientBeatInterval");
        }
    }
    catch (NacosException &e) {
        NacosString jsonBeatInfo = JSON::toJSONString(beatInfo);
        log_error("[CLIENT-BEAT] failed to send beat: %s e:%s\n", jsonBeatInfo.c_str(), e.what());
    }
    return 0L;
}