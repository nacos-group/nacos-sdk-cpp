#include <map>
#include "NamingProxy.h"
#include "constant/NamingConstant.h"
#include "constant/UtilAndComs.h"
#include "src/utils/UuidUtils.h"
#include "src/utils/ParamUtils.h"
#include "src/utils/RandomUtils.h"
#include "src/json/JSON.h"
#include "src/http/HttpStatus.h"
#include "NacosExceptions.h"
#include "src/crypto/SignatureTool.h"

using namespace std;

namespace nacos{

ListView<NacosString> NamingProxy::nullResult;

NamingProxy::NamingProxy(ObjectConfigData *objectConfigData) {
    _objectConfigData = objectConfigData;
    log_debug("NamingProxy Constructor:\n"
              "namespace:%s, endpoint:%s, Servers:%s\n",
              objectConfigData->_serverListManager->getNamespace().c_str(), objectConfigData->_serverListManager->getEndpoint().c_str(),
              objectConfigData->_serverListManager->toString().c_str());
    serverPort = "8848";

    if (objectConfigData->_serverListManager->getServerCount() == 1) {
        nacosDomain = objectConfigData->_serverListManager->getServerList().begin()->getCompleteAddress();
    }
    log_debug("The serverlist:%s\n", objectConfigData->_serverListManager->toString().c_str());

    nullResult.setCount(0);
    _hb_fail_wait = atoi(objectConfigData->_appConfigManager->get(PropertyKeyConst::HB_FAIL_WAIT_TIME).c_str());
}

NamingProxy::~NamingProxy() {
}

void NamingProxy::registerService(const NacosString &serviceName, const NacosString &groupName,
                                  Instance &instance) NACOS_THROW(NacosException) {
    log_info("[REGISTER-SERVICE] %s registering service %s with instance: %s\n",
             getNamespaceId().c_str(), serviceName.c_str(), instance.toString().c_str());

    list <NacosString> params;
    ParamUtils::addKV(params, NamingConstant::NAMESPACE_ID, getNamespaceId());
    ParamUtils::addKV(params, NamingConstant::SERVICE_NAME, serviceName);
    ParamUtils::addKV(params, NamingConstant::GROUP_NAME, groupName);
    ParamUtils::addKV(params, NamingConstant::CLUSTER_NAME, instance.clusterName);
    ParamUtils::addKV(params, "ip", instance.ip);
    ParamUtils::addKV(params, "port",  NacosStringOps::valueOf(instance.port));
    ParamUtils::addKV(params, "weight", NacosStringOps::valueOf(instance.weight));
    ParamUtils::addKV(params, "enable", NacosStringOps::valueOf(instance.enabled));
    ParamUtils::addKV(params, NamingConstant::HEALTHY, NacosStringOps::valueOf(instance.healthy));
    ParamUtils::addKV(params, "ephemeral", NacosStringOps::valueOf(instance.ephemeral));
    ParamUtils::addKV(params, "metadata", JSON::toJSONString(instance.metadata));

    reqAPI("/" + _objectConfigData->_appConfigManager->getContextPath() + UtilAndComs::NACOS_URL_INSTANCE, params, IHttpCli::POST);
}

void NamingProxy::deregisterService(const NacosString &serviceName, Instance &instance) NACOS_THROW(NacosException) {
    log_info("[DEREGISTER-SERVICE] %s deregistering service %s with instance: %s\n",
             getNamespaceId().c_str(), serviceName.c_str(), instance.toString().c_str());

    list <NacosString> params;
    ParamUtils::addKV(params, NamingConstant::NAMESPACE_ID, getNamespaceId());
    ParamUtils::addKV(params, NamingConstant::SERVICE_NAME, serviceName);
    ParamUtils::addKV(params, NamingConstant::CLUSTER_NAME, instance.clusterName);
    ParamUtils::addKV(params, "ip", instance.ip);
    ParamUtils::addKV(params, "port",  NacosStringOps::valueOf(instance.port));
    ParamUtils::addKV(params, "ephemeral", NacosStringOps::valueOf(instance.ephemeral));

    reqAPI("/" + _objectConfigData->_appConfigManager->getContextPath() + UtilAndComs::NACOS_URL_INSTANCE, params, IHttpCli::DELETE);
}

NacosString NamingProxy::queryList(const NacosString &serviceName, const NacosString &groupName, const NacosString &clusters,
                                    int udpPort, bool healthyOnly) NACOS_THROW(NacosException) {
    list <NacosString> params;
    const NacosString &localIp = _objectConfigData->_appConfigManager->get(PropertyKeyConst::LOCAL_IP);
    ParamUtils::addKV(params, NamingConstant::NAMESPACE_ID, getNamespaceId());
    ParamUtils::addKV(params, NamingConstant::SERVICE_NAME, serviceName);
    ParamUtils::addKV(params, NamingConstant::GROUP_NAME, groupName);
    ParamUtils::addKV(params, NamingConstant::CLUSTERS, clusters);
    ParamUtils::addKV(params, NamingConstant::UDP_PORT, NacosStringOps::valueOf(udpPort));
    ParamUtils::addKV(params, NamingConstant::CLIENT_IP, localIp);
    ParamUtils::addKV(params, NamingConstant::HEALTHY_ONLY, NacosStringOps::valueOf(healthyOnly));

    return reqAPI("/" + _objectConfigData->_appConfigManager->getContextPath() + UtilAndComs::NACOS_URL_BASE + "/instance/list", params, IHttpCli::GET);
}

NacosString
NamingProxy::reqAPI(const NacosString &api, list <NacosString> &params, int method) NACOS_THROW(NacosException) {
    ServerListManager *_serverListManager = _objectConfigData->_serverListManager;
    list <NacosServerInfo> servers = _serverListManager->getServerList();

    if (_serverListManager->getServerCount() == 0) {
        throw NacosException(NacosException::NO_SERVER_AVAILABLE, "no server available");
    }

    NacosString errmsg;
    if (!servers.empty()) {
        size_t maxSvrSlot = servers.size();
        log_debug("nr_servers:%d\n", maxSvrSlot);
        size_t selectedServer = RandomUtils::random(0, maxSvrSlot) % maxSvrSlot;
        log_debug("selected_server:%d\n", selectedServer);

        for (size_t i = 0; i < servers.size(); i++) {
            const NacosServerInfo &server = ParamUtils::getNthElem(servers, selectedServer);
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

        throw NacosException(NacosException::ALL_SERVERS_TRIED_AND_FAILED, "failed to req API:" + api + " after all servers(" + _serverListManager->toString() +
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

    throw NacosException(NacosException::ALL_SERVERS_TRIED_AND_FAILED, "failed to req API:/api/" + api + " after all servers(" + _serverListManager->toString() +
                            ") tried: " + errmsg);
}

NacosString NamingProxy::callServer
        (
                const NacosString &api,
                list <NacosString> &params,
                const NacosString &curServer
        ) NACOS_THROW(NacosException) {
    return callServer(api, params, nacosDomain, IHttpCli::GET);
}

NacosString NamingProxy::getDataToSign(const std::list <NacosString> &paramValues, NacosString &nowTimeMs) {
    const NacosString &serviceName = ParamUtils::findByKey(paramValues, NamingConstant::SERVICE_NAME);
    const NacosString &groupName = ParamUtils::findByKey(paramValues, NamingConstant::GROUP_NAME);

    NacosString dataToSign = "";
    if ((!ParamUtils::isBlank(serviceName) && serviceName.find(NamingConstant::SPLITER) != std::string::npos) || ParamUtils::isBlank(groupName)) {
        dataToSign = nowTimeMs + NamingConstant::SPLITER + serviceName;
    } else {
        dataToSign = nowTimeMs + NamingConstant::SPLITER + groupName + NamingConstant::SPLITER + serviceName;
    }
    return dataToSign;
}

NacosString NamingProxy::callServer
        (
                const NacosString &api,
                list <NacosString> &params,
                const NacosString &curServer,
                int method
        ) NACOS_THROW(NacosException) {
    NacosString requestUrl;
    //Current server address doesn't have SERVER_ADDR_IP_SPLITER, which means
    if (!ParamUtils::contains(curServer, UtilAndComs::SERVER_ADDR_IP_SPLITER)) {
        requestUrl = curServer + UtilAndComs::SERVER_ADDR_IP_SPLITER + serverPort;
    } else {
        requestUrl = curServer;
    }

    //TODO:http/https implementation
    requestUrl = requestUrl + api;

    HttpResult requestRes;
    list <NacosString> headers;
    headers = builderHeaders();

    //SPAS security
    NacosString secretKey = _objectConfigData->_appConfigManager->get(PropertyKeyConst::SECRET_KEY);
    NacosString accessKey = _objectConfigData->_appConfigManager->get(PropertyKeyConst::ACCESS_KEY);

    //If SPAS security credentials are set, SPAS is enabled
    if (!ParamUtils::isBlank(secretKey) && !ParamUtils::isBlank(accessKey)) {
        NacosString nowTimeMs = NacosStringOps::valueOf(TimeUtils::getCurrentTimeInMs());
        NacosString dataToSign = getDataToSign(params, nowTimeMs);
        NacosString signature = SignatureTool::SignWithHMAC_SHA1(dataToSign, secretKey);
        //inject security credentials
        if (method == IHttpCli::GET || method == IHttpCli::DELETE) {
            ParamUtils::addKV(params, "signature", signature);
            ParamUtils::addKV(params, "data", dataToSign);
            ParamUtils::addKV(params, "ak", accessKey);
        } else {
            size_t pos = 0;
            NacosString from = "+";
            NacosString to = "%2B";
            while((pos = signature.find(from, pos)) != std::string::npos) {
                signature.replace(pos,  from.size(), to);
                pos += to.size();
            }
            requestUrl = requestUrl + "?signature=" + signature + "&data=" + dataToSign + "&ak=" + accessKey;
        }
    }

    HttpDelegate *_httpDelegate = _objectConfigData->_httpDelegate;
    
    try {
        long _http_req_timeout = _objectConfigData->_appConfigManager->getServeReqTimeout();
        switch (method) {
            case IHttpCli::GET:
                requestRes = _httpDelegate->httpGet(requestUrl, headers, params, UtilAndComs::ENCODING,
                                              _http_req_timeout);
                break;
            case IHttpCli::PUT:
                requestRes = _httpDelegate->httpPut(requestUrl, headers, params, UtilAndComs::ENCODING,
                                              _http_req_timeout);
                break;
            case IHttpCli::POST:
                requestRes = _httpDelegate->httpPost(requestUrl, headers, params, UtilAndComs::ENCODING,
                                               _http_req_timeout);
                break;
            case IHttpCli::DELETE:
                requestRes = _httpDelegate->httpDelete(requestUrl, headers, params, UtilAndComs::ENCODING,
                                                 _http_req_timeout);
                break;
        }
    }
    catch (NetworkException &e) {
        NacosString errMsg = "Failed to request server, ";
        errMsg += e.what();
        throw NacosException(NacosException::SERVER_ERROR, errMsg);
    }

    if (requestRes.code == HttpStatus::HTTP_OK) {
        return requestRes.content;
    }

    if (requestRes.code == HttpStatus::HTTP_NOT_MODIFIED) {
        return NULLSTR;
    }
    //TODO:Metrics & Monitoring

    throw NacosException(requestRes.code,
                         "failed to req API:" + requestUrl + " code:" + NacosStringOps::valueOf(requestRes.code) +
                         " errormsg:" + requestRes.content);
}

inline NacosString NamingProxy::getNamespaceId() {
    return _objectConfigData->_appConfigManager->get(PropertyKeyConst::NAMESPACE);
}

list <NacosString> NamingProxy::builderHeaders() {
    list <NacosString> headers;
    headers.push_back("Client-Version");
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
        list <NacosString> params;
        ParamUtils::addKV(params, NamingConstant::BEAT, JSON::toJSONString(beatInfo));
        ParamUtils::addKV(params, NamingConstant::NAMESPACE_ID, getNamespaceId());
        ParamUtils::addKV(params, NamingConstant::SERVICE_NAME, beatInfo.serviceName);
        NacosString result = reqAPI("/" + _objectConfigData->_appConfigManager->getContextPath() + UtilAndComs::NACOS_URL_BASE + "/instance/beat", params, IHttpCli::PUT);
        //JSONObject jsonObject = JSON.parseObject(result);

        if (!isNull(result)) {
            return JSON::getLong(result, "clientBeatInterval");
        }
    }
    catch (NacosException &e) {
        NacosString jsonBeatInfo = JSON::toJSONString(beatInfo);
        log_error("[CLIENT-BEAT] failed to send beat: %s e:%s\n", jsonBeatInfo.c_str(), e.what());
        return _hb_fail_wait;
    }
    return 0L;
}

ListView<NacosString> NamingProxy::getServiceList(int page, int pageSize, const NacosString &groupName) NACOS_THROW(NacosException)
{
    log_debug("[NAMEPRXY] request:group=%s page=%d pageSize=%d\n", groupName.c_str(), page, pageSize);
    list <NacosString> params;
    ParamUtils::addKV(params, NamingConstant::PAGE_NO, NacosStringOps::valueOf(page));
    ParamUtils::addKV(params, NamingConstant::PAGE_SIZE, NacosStringOps::valueOf(pageSize));
    ParamUtils::addKV(params, NamingConstant::GROUP_NAME, groupName);
    ParamUtils::addKV(params, NamingConstant::NAMESPACE_ID, getNamespaceId());
    NacosString result = reqAPI("/" + _objectConfigData->_appConfigManager->getContextPath() + UtilAndComs::NACOS_URL_BASE + "/service/list", params, IHttpCli::GET);

    if (!isNull(result)) {
        return JSON::Json2ServiceList(result);
    }

    return nullResult;
}

ServiceInfo2 NamingProxy::getServiceInfo(const NacosString &serviceName, const NacosString &groupName) NACOS_THROW(NacosException)
{
    log_debug("[NAMEPRXY] getServiceInfo request:serviceName=%s groupName=%s\n",
              serviceName.c_str(), groupName.c_str());
    list <NacosString> params;
    ParamUtils::addKV(params, NamingConstant::SERVICE_NAME, serviceName);
    if (!NacosStringOps::isNullStr(groupName)) {
        ParamUtils::addKV(params, NamingConstant::GROUP_NAME, groupName);
    } else {
        ParamUtils::addKV(params, NamingConstant::GROUP_NAME, ConfigConstant::DEFAULT_GROUP);
    }
    ParamUtils::addKV(params, NamingConstant::NAMESPACE_ID, getNamespaceId());
    NacosString result = reqAPI("/" + _objectConfigData->_appConfigManager->getContextPath() + UtilAndComs::NACOS_URL_BASE + "/service", params, IHttpCli::GET);
    log_debug("NamingProxy::getServiceInfo: service info from server:%s\n", result.c_str());

    if (!isNull(result)) {
        return JSON::Json2ServiceInfo2(result);
    }

    return ServiceInfo2::nullServiceInfo2;
}

bool areYouOk(const NacosString &imVeryOk) {
    if (imVeryOk.compare("ok") == 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * removes a service info from nacos server
 * please note that the operation will succeed ONLY WHEN there's NO instance of the service specified
 *
 * @param serviceName service name
 * @param groupName   group name
 * @return true if operation succeeds
 *         otherwise return false
 * @throws IOException IOException
 */
bool NamingProxy::deleteServiceInfo(const NacosString &serviceName, const NacosString &groupName) NACOS_THROW(NacosException) {
    list<NacosString> params;
    ParamUtils::addKV(params, NamingConstant::SERVICE_NAME, serviceName);
    ParamUtils::addKV(params, NamingConstant::GROUP_NAME, groupName);
    ParamUtils::addKV(params, NamingConstant::NAMESPACE_ID, getNamespaceId());
    NacosString result = reqAPI("/" + _objectConfigData->_appConfigManager->getContextPath() + UtilAndComs::NACOS_URL_BASE + "/service", params, IHttpCli::DELETE);

    return areYouOk(result);
}

void assembleServiceInfoRequest(list<NacosString> &target, const ServiceInfo2 &serviceInfo2) {
    if (serviceInfo2.isNameSet()) {
        ParamUtils::addKV(target, NamingConstant::SERVICE_NAME, serviceInfo2.getName());
    }
    if (serviceInfo2.isGroupNameSet()) {
        ParamUtils::addKV(target, NamingConstant::GROUP_NAME, serviceInfo2.getGroupName());
    }
    if (serviceInfo2.isNamespaceIdSet()) {
        ParamUtils::addKV(target, NamingConstant::NAMESPACE_ID, serviceInfo2.getNamespaceId());
    }
    if (serviceInfo2.isProtectThresholdSet()) {
        ParamUtils::addKV(target, "protectThreshold", NacosStringOps::valueOf(serviceInfo2.getProtectThreshold()));
    }
    if (serviceInfo2.isMetadataSet()) {
        ParamUtils::addKV(target, "metadata", JSON::toJSONString(serviceInfo2.getMetadata()));
    }
}

bool NamingProxy::createServiceInfo(const ServiceInfo2 &serviceInfo2, naming::Selector *selector) NACOS_THROW(NacosException) {
    list<NacosString> params;
    assembleServiceInfoRequest(params, serviceInfo2);
    if (selector) {
        ParamUtils::addKV(params, "selector", selector->getSelectorString());
    }

    NacosString result = reqAPI("/" + _objectConfigData->_appConfigManager->getContextPath() + UtilAndComs::NACOS_URL_BASE + "/service", params, IHttpCli::POST);
    return areYouOk(result);
}

bool NamingProxy::updateServiceInfo(const ServiceInfo2 &serviceInfo2, naming::Selector *selector) NACOS_THROW(NacosException) {
    list<NacosString> params;
    assembleServiceInfoRequest(params, serviceInfo2);
    if (selector) {
        ParamUtils::addKV(params, "selector", selector->getSelectorString());
    }

    NacosString result = reqAPI("/" + _objectConfigData->_appConfigManager->getContextPath() + UtilAndComs::NACOS_URL_BASE + "/service", params, IHttpCli::PUT);
    return areYouOk(result);
}

bool NamingProxy::serverHealthy() {
    list<NacosString> params;
    NacosString result = reqAPI("/" + _objectConfigData->_appConfigManager->getContextPath() + UtilAndComs::NACOS_URL_BASE + "/operator/metrics", params, IHttpCli::GET);
    NacosString healthyTag = "status\":\"";
    size_t pos = result.find(healthyTag);
    if (pos == std::string::npos) {
        return false;
    }

    NacosString healthy = result.substr(pos + healthyTag.length(), 2);
    return healthy == "UP";
}

/**
 * gets one service instance info from nacos server
 *
 * @param serviceName service name
 * @param ip          ip address
 * @param port        port
 * @param params      optional parameters, options are:
 * groupName
 * namespaceId
 * cluster
 * healthyOnly
 * ephemeral
 * @return Instance info if succeed, nullObj if fails
 * @throws NacosException NacosException
 */
Instance NamingProxy::getServiceInstance
(
    const NacosString &serviceName,
    const NacosString &ip,
    int port,
    const std::map<NacosString, NacosString> &params
)
NACOS_THROW(NacosException) {
    list<NacosString> paramsList;
    ParamUtils::addKV(paramsList, NamingConstant::SERVICE_NAME, serviceName);
    ParamUtils::addKV(paramsList, "ip", ip);
    ParamUtils::addKV(paramsList, "port", NacosStringOps::valueOf(port));

    for (map<NacosString, NacosString>::const_iterator it = params.begin();
        it != params.end(); it++) {
        ParamUtils::addKV(paramsList, it->first, it->second);
    }

    if (params.count(NamingConstant::NAMESPACE_ID) == 0) {
        ParamUtils::addKV(paramsList, NamingConstant::NAMESPACE_ID, getNamespaceId());
    }

    if (params.count(NamingConstant::GROUP_NAME) == 0) {
        ParamUtils::addKV(paramsList, NamingConstant::GROUP_NAME, ConfigConstant::DEFAULT_GROUP);
    }

    NacosString result = reqAPI("/" + _objectConfigData->_appConfigManager->getContextPath() + UtilAndComs::NACOS_URL_BASE + "/instance", paramsList, IHttpCli::GET);

    return JSON::Json2Instance(result);
}

bool NamingProxy::updateServiceInstance(const Instance &instance) NACOS_THROW(NacosException) {
    list<NacosString> params;
    ParamUtils::addKV(params, NamingConstant::SERVICE_NAME, instance.serviceName);
    ParamUtils::addKV(params, "ip", instance.ip);
    ParamUtils::addKV(params, "port", NacosStringOps::valueOf(instance.port));

    if (NacosStringOps::isNullStr(instance.groupName)) {
        ParamUtils::addKV(params, NamingConstant::GROUP_NAME, ConfigConstant::DEFAULT_GROUP);
    } else {
        ParamUtils::addKV(params, NamingConstant::GROUP_NAME, instance.groupName);
    }

    if (NacosStringOps::isNullStr(instance.namespaceId)) {
        ParamUtils::addKV(params, NamingConstant::NAMESPACE_ID, getNamespaceId());
    } else {
        ParamUtils::addKV(params, NamingConstant::NAMESPACE_ID, instance.namespaceId);
    }

    if (!NacosStringOps::isNullStr(instance.clusterName)) {
        ParamUtils::addKV(params, NamingConstant::CLUSTER_NAME, instance.clusterName);
    }

    if (instance.metadata.size() > 0) {
        ParamUtils::addKV(params, "metadata", JSON::toJSONString(instance.metadata));
    }
    ParamUtils::addKV(params, "ephemeral", NacosStringOps::valueOf(instance.ephemeral));
    //TODO:weight is optional
    ParamUtils::addKV(params, "weight", NacosStringOps::valueOf(instance.weight));

    NacosString result = reqAPI("/" + _objectConfigData->_appConfigManager->getContextPath() + UtilAndComs::NACOS_URL_BASE + "/instance", params, IHttpCli::PUT);

    return areYouOk(result);
}
}//namespace nacos
