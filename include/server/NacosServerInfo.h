//
// Created by Liu, Hanyu on 2020/9/4.
//

#ifndef NACOS_SDK_CPP_NACOSSERVERINFO_H
#define NACOS_SDK_CPP_NACOSSERVERINFO_H

#include <ostream>
#include "NacosString.h"

class NacosServerInfo
{
private:
    NacosString ip;
    int port;
    NacosString site;
    float weight;
    float adWeight;
    bool alive;
    long lastRefTime;
    NacosString lastRefTimeStr;
    NacosString key;
public:
    const NacosString getIp() const {
        return ip;
    }

    void setIp(const NacosString &ip) {
        NacosServerInfo::ip = ip;
    }

    int getPort() const {
        return port;
    }

    void setPort(int port) {
        NacosServerInfo::port = port;
    }

    const NacosString getSite() const {
        return site;
    }

    void setSite(const NacosString &site) {
        NacosServerInfo::site = site;
    }

    float getWeight() const {
        return weight;
    }

    void setWeight(float weight) {
        NacosServerInfo::weight = weight;
    }

    float getAdWeight() const {
        return adWeight;
    }

    void setAdWeight(float adWeight) {
        NacosServerInfo::adWeight = adWeight;
    }

    bool isAlive() const {
        return alive;
    }

    void setAlive(bool alive) {
        NacosServerInfo::alive = alive;
    }

    long getLastRefTime() const {
        return lastRefTime;
    }

    void setLastRefTime(long lastRefTime) {
        NacosServerInfo::lastRefTime = lastRefTime;
    }

    const NacosString getLastRefTimeStr() const {
        return lastRefTimeStr;
    }

    void setLastRefTimeStr(const NacosString &lastRefTimeStr) {
        NacosServerInfo::lastRefTimeStr = lastRefTimeStr;
    }

    const NacosString getKey() const {
        return key;
    }

    void setKey(const NacosString &key) {
        NacosServerInfo::key = key;
    }

    NacosString toString() {
        NacosString res;
        res = ip + NacosStringOps::valueOf(port);
        return res;
    }
};

#endif //NACOS_SDK_CPP_NACOSSERVERINFO_H
