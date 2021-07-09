#ifndef __JSON_H_
#define __JSON_H_

#include <map>
#include "NacosString.h"
#include "src/naming/beat/BeatInfo.h"
#include "naming/ServiceInfo.h"
#include "src/json/rapidjson/document.h"
#include "src/json/rapidjson/writer.h"
#include "src/json/rapidjson/stringbuffer.h"
#include "naming/Instance.h"
#include "src/server/NacosServerInfo.h"
#include "naming/ListView.h"
#include "naming/ServiceInfo2.h"
#include "src/security/SecurityManager.h"
#include "src/naming/subscribe/UdpNamingServiceListener.h"
#include "Compatibility.h"

/**
 * JSON
 *
 * @author Liu, Hanyu
 * Adapter from nacos-sdk-cpp to a json parser
 */
namespace nacos{
class JSON {
public:
    static NacosString toJSONString(BeatInfo &beatInfo);

    static NacosString toJSONString(const std::map <NacosString, NacosString> &mapinfo);

    static void Map2JSONObject(rapidjson::Document &d, rapidjson::Value &jsonOb, std::map <NacosString, NacosString> &mapinfo);

    static void JSONObject2Map(std::map <NacosString, NacosString> &mapinfo, const rapidjson::Value &jsonOb);

    static long getLong(const NacosString &jsonString, const NacosString &fieldname);

    static ServiceInfo JsonStr2ServiceInfo(const NacosString &jsonString) NACOS_THROW(NacosException);

    static Instance Json2Instance(const rapidjson::Value &jsonString) NACOS_THROW(NacosException);

    static Instance Json2Instance(const NacosString &jsonString) NACOS_THROW(NacosException);

    static void markRequired(const rapidjson::Document &d, const NacosString &requiredField) NACOS_THROW(NacosException);

    static void markRequired(const rapidjson::Value &d, const NacosString &requiredField) NACOS_THROW(NacosException);

    static std::list<NacosServerInfo> Json2NacosServerInfo(const NacosString &nacosString) NACOS_THROW(NacosException);

    static ServiceInfo2 Json2ServiceInfo2(const NacosString &nacosString) NACOS_THROW(NacosException);

    static ListView<NacosString> Json2ServiceList(const NacosString &nacosString) NACOS_THROW(NacosException);

    static AccessToken Json2AccessToken(const NacosString &nacosString) NACOS_THROW(NacosException);

    static PushPacket Json2PushPacket(const char *jsonString) NACOS_THROW(NacosException);
};
}//namespace nacos

#endif