//
// Created by Liu, Hanyu on 2020/9/4.
//

#ifndef NACOS_SDK_CPP_SERVERINFO_H
#define NACOS_SDK_CPP_SERVERINFO_H
#include "NacosString.h"

class ServerInfo
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
    const NacosString &getIp() const {
        return ip;
    }

    void setIp(const NacosString &ip) {
        ServerInfo::ip = ip;
    }

    int getPort() const {
        return port;
    }

    void setPort(int port) {
        ServerInfo::port = port;
    }

    const NacosString &getSite() const {
        return site;
    }

    void setSite(const NacosString &site) {
        ServerInfo::site = site;
    }

    float getWeight() const {
        return weight;
    }

    void setWeight(float weight) {
        ServerInfo::weight = weight;
    }

    float getAdWeight() const {
        return adWeight;
    }

    void setAdWeight(float adWeight) {
        ServerInfo::adWeight = adWeight;
    }

    bool isAlive() const {
        return alive;
    }

    void setAlive(bool alive) {
        ServerInfo::alive = alive;
    }

    long getLastRefTime() const {
        return lastRefTime;
    }

    void setLastRefTime(long lastRefTime) {
        ServerInfo::lastRefTime = lastRefTime;
    }

    const NacosString &getLastRefTimeStr() const {
        return lastRefTimeStr;
    }

    void setLastRefTimeStr(const NacosString &lastRefTimeStr) {
        ServerInfo::lastRefTimeStr = lastRefTimeStr;
    }

    const NacosString &getKey() const {
        return key;
    }

    void setKey(const NacosString &key) {
        ServerInfo::key = key;
    }
};

#endif //NACOS_SDK_CPP_SERVERINFO_H
