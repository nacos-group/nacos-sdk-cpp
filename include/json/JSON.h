#ifndef __JSON_H_
#define __JSON_H_

#include <map>
#include "NacosString.h"
#include "naming/beat/BeatInfo.h"
#include "naming/ServiceInfo.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "naming/Instance.h"
#include "server/NacosServerInfo.h"

/**
 * JSON
 *
 * @author Liu, Hanyu
 * Adapter from nacos-cpp-cli to a json parser
 */
class JSON {
public:
    static NacosString toJSONString(BeatInfo &beatInfo);

    static NacosString toJSONString(std::map <NacosString, NacosString> &mapinfo);

    static void Map2JSONObject(rapidjson::Value &jsonOb, std::map <NacosString, NacosString> &mapinfo);

    static void JSONObject2Map(std::map <NacosString, NacosString> &mapinfo, const rapidjson::Value &jsonOb);

    static long getLong(const NacosString &jsonString, const NacosString &fieldname);

    static ServiceInfo JsonStr2ServiceInfo(const NacosString &jsonString) throw(NacosException);

    static Instance Json2Instance(const rapidjson::Value &jsonString) throw(NacosException);

    static void markRequired(const rapidjson::Document &d, const NacosString &requiredField) throw(NacosException);

    static void markRequired(const rapidjson::Value &d, const NacosString &requiredField) throw(NacosException);

    static std::list <NacosServerInfo> Json2NacosServerInfo(const NacosString &nacosString) throw(NacosException);
};


#endif