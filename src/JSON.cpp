#include <iostream>
#include "json/JSON.h"
#include "naming/beat/BeatInfo.h"
#include "NacosString.h"

/**
 * JSON
 *
 * @author Liu, Hanyu
 * Adapter from nacos-cpp-cli to a json parser
 */
using namespace std;
using namespace rapidjson;

NacosString documentToString(Document &d) {
    StringBuffer buffer;
    Writer <StringBuffer> writer(buffer);
    d.Accept(writer);
    NacosString result = buffer.GetString();
    return result;
}

NacosString JSON::toJSONString(map <NacosString, NacosString> &mapinfo) {
    Document d;
    d.SetObject();
    for (map<NacosString, NacosString>::iterator it = mapinfo.begin(); it != mapinfo.end(); it++) {
        Value k;
        k.SetString(it->first.c_str(), d.GetAllocator());
        Value v;
        v.SetString(it->second.c_str(), d.GetAllocator());
        d.AddMember(k, v, d.GetAllocator());
    }


    return documentToString(d);
}

void JSON::Map2JSONObject(Value &jsonOb, map <NacosString, NacosString> &mapinfo) {
    Document document;
    jsonOb.SetObject();
    for (map<NacosString, NacosString>::iterator it = mapinfo.begin(); it != mapinfo.end(); it++) {
        Value k;
        k.SetString(it->first.c_str(), document.GetAllocator());
        Value v;
        v.SetString(it->second.c_str(), document.GetAllocator());
        jsonOb.AddMember(k, v, document.GetAllocator());
    }
}

void JSON::JSONObject2Map(std::map <NacosString, NacosString> &mapinfo, const Value &jsonOb) {
    for (Value::ConstMemberIterator iter = jsonOb.MemberBegin(); iter != jsonOb.MemberEnd(); ++iter) {
        NacosString name = iter->name.GetString();
        NacosString value = iter->value.GetString();
        mapinfo[name] = value;
    }
}

//Add key-value
void AddKV(Document &d, const NacosString &k, const NacosString &v) {
    Value k_, v_;
    k_.SetString(k.c_str(), d.GetAllocator());
    v_.SetString(v.c_str(), d.GetAllocator());
    d.AddMember(k_, v_, d.GetAllocator());
}

//Add key-Object
void AddKO(Document &d, const NacosString &k, Value &o) {
    Value k_;
    k_.SetString(k.c_str(), d.GetAllocator());
    d.AddMember(k_, o, d.GetAllocator());
}

NacosString JSON::toJSONString(BeatInfo &beatInfo) {
    Document d;
    d.SetObject();
    AddKV(d, "port", NacosStringOps::valueOf(beatInfo.port));
    AddKV(d, "ip", beatInfo.ip);
    AddKV(d, "weight", NacosStringOps::valueOf(beatInfo.weight));
    AddKV(d, "serviceName", beatInfo.serviceName);
    AddKV(d, "cluster", beatInfo.cluster);
    AddKV(d, "scheduled", NacosStringOps::valueOf(beatInfo.scheduled));
    Value metadata;
    Map2JSONObject(metadata, beatInfo.metadata);
    AddKO(d, "metadata", metadata);

    //d["port"] = NacosStringOps::valueOf(beatInfo.port);
    //d["ip"] = beatInfo.ip;
    //d["weight"] = NacosStringOps::valueOf(beatInfo.weight);
    //d["serviceName"] = beatInfo.serviceName;
    //d["cluster"] = beatInfo.cluster;
    //d["scheduled"] = beatInfo.scheduled;
    //d["metadata"] = toJSONString(beatInfo.metadata);
    return documentToString(d);
}

long JSON::getLong(const NacosString &jsonString, const NacosString &fieldname) {
    Document d;
    d.Parse(jsonString.c_str());
    Value &s = d[fieldname.c_str()];
    return s.GetInt64();
}

Instance JSON::Json2Instance(const Value &host) throw(NacosException) {
    Instance theinstance;

    markRequired(host, "instanceId");
    const Value &instanceId = host["instanceId"];
    theinstance.instanceId = instanceId.GetString();

    markRequired(host, "port");
    const Value &port = host["port"];
    if (!port.IsInt()) {
        throw NacosException(NacosException::INVALID_JSON_FORMAT, "Error while parsing port for Instance!");
    }
    theinstance.port = port.GetInt();

    markRequired(host, "ip");
    const Value &ip = host["ip"];
    theinstance.ip = ip.GetString();

    markRequired(host, "weight");
    const Value &weight = host["weight"];
    if (!weight.IsDouble()) {
        throw NacosException(NacosException::INVALID_JSON_FORMAT, "Error while parsing weight for Instance!");
    }
    theinstance.weight = weight.GetDouble();

    markRequired(host, "metadata");
    const Value &metadata = host["metadata"];

    std::map <NacosString, NacosString> mtdata;
    JSONObject2Map(mtdata, metadata);

    theinstance.metadata = mtdata;

    return theinstance;
}

void JSON::markRequired(const Document &d, const NacosString &requiredField) throw(NacosException) {
    if (!d.HasMember(requiredField.c_str())) {
        throw NacosException(NacosException::LACK_JSON_FIELD, "Missing required field:" + requiredField);
    }
}

void JSON::markRequired(const Value &v, const NacosString &requiredField) throw(NacosException) {
    if (!v.HasMember(requiredField.c_str())) {
        throw NacosException(NacosException::LACK_JSON_FIELD, "Missing required field:" + requiredField);
    }
}

ServiceInfo JSON::JsonStr2ServiceInfo(const NacosString &jsonString) throw(NacosException) {
    ServiceInfo si;
    Document d;
    d.Parse(jsonString.c_str());

    if (d.HasParseError()) {
        throw NacosException(NacosException::INVALID_JSON_FORMAT,
                             "Error while parsing the JSON String for ServiceInfo!");
    }

    markRequired(d, "cacheMillis");
    const Value &cacheMillis = d["cacheMillis"];
    if (!cacheMillis.IsInt64()) {
        throw NacosException(NacosException::INVALID_JSON_FORMAT, "Error while parsing cacheMillis for ServiceInfo!");
    }
    si.setCacheMillis(cacheMillis.GetInt64());

    markRequired(d, "checksum");
    const Value &checkSum = d["checksum"];
    si.setChecksum(checkSum.GetString());

    markRequired(d, "lastRefTime");
    const Value &lastRefTime = d["lastRefTime"];
    if (!lastRefTime.IsInt64()) {
        throw NacosException(NacosException::INVALID_JSON_FORMAT, "Error while parsing lastRefTime for ServiceInfo!");
    }
    si.setLastRefTime(lastRefTime.GetInt64());

    markRequired(d, "clusters");
    const Value &clusters = d["clusters"];
    si.setClusters(clusters.GetString());

    markRequired(d, "hosts");
    const Value &hosts = d["hosts"];
    std::list <Instance> hostlist;
    for (SizeType i = 0; i < hosts.Size(); i++) {
        const Value &curhost = hosts[i];
        Instance curinstance = Json2Instance(curhost);
        hostlist.push_back(curinstance);
    }

    si.setHosts(hostlist);

    return si;
}

NacosServerInfo parseOneNacosSvr(const Value &curSvr) {
    NacosServerInfo res;
    res.setIp(curSvr["ip"].GetString());
    res.setPort(curSvr["servePort"].GetInt());
    res.setSite(curSvr["site"].GetString());
    res.setWeight(curSvr["weight"].GetFloat());
    res.setAdWeight(curSvr["adWeight"].GetFloat());
    res.setAlive(curSvr["alive"].GetBool());
    res.setLastRefTime(curSvr["lastRefTime"].GetInt64());
    if (!curSvr["lastRefTimeStr"].IsNull()) {
        res.setLastRefTimeStr(curSvr["lastRefTimeStr"].GetString());
    }
    res.setKey(curSvr["key"].GetString());
    return res;
}

list <NacosServerInfo> JSON::Json2NacosServerInfo(const NacosString &nacosString) throw(NacosException) {
    list <NacosServerInfo> nacosServerList;
    ServiceInfo si;
    Document d;
    d.Parse(nacosString.c_str());

    if (d.HasParseError()) {
        throw NacosException(NacosException::INVALID_JSON_FORMAT,
                             "Error while parsing the JSON String for NacosServerInfo!");
    }

    markRequired(d, "servers");
    const Value &servers = d["servers"];
    if (!servers.IsArray()) {
        throw NacosException(NacosException::INVALID_JSON_FORMAT, "Error while parsing servers for NacosServerInfo!");
    }

    for (SizeType i = 0; i < servers.Size(); i++) {
        const Value &curSvr = servers[i];
        NacosServerInfo curSvrInfo = parseOneNacosSvr(curSvr);
        nacosServerList.push_back(curSvrInfo);
    }

    return nacosServerList;
}

ListView<NacosString> JSON::Json2ServiceList(const NacosString &nacosString) throw(NacosException) {
    ListView<NacosString> serviceList;
    ServiceInfo si;
    Document d;
    d.Parse(nacosString.c_str());

    if (d.HasParseError()) {
        throw NacosException(NacosException::INVALID_JSON_FORMAT,
                             "Error while parsing the JSON String for ServiceList!");
    }

    markRequired(d, "count");
    markRequired(d, "doms");
    const Value &count = d["count"];
    if (!count.IsInt()) {
        throw NacosException(NacosException::INVALID_JSON_FORMAT, "Error while parsing servers for ServiceList.count!");
    }

    serviceList.setCount(count.GetInt());

    const Value &doms = d["doms"];
    if (!doms.IsArray()) {
        throw NacosException(NacosException::INVALID_JSON_FORMAT, "Error while parsing servers for ServiceList.doms!");
    }

    list<NacosString> names;

    for (SizeType i = 0; i < doms.Size(); i++) {
        const Value &curName = doms[i];
        names.push_back(curName.GetString());
    }
    serviceList.setData(names);

    return serviceList;
}