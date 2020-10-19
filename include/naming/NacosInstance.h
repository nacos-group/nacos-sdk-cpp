#ifndef __NACOS_INSTANCE_H_
#define __NACOS_INSTANCE_H_

#include <map>
#include "NacosString.h"

namespace nacos{
class NacosInstance {
public:

    /**
    * instance ip
    */
    NacosString ip;

    /**
    * instance port
    */
    int port;

    NacosString state;

    //extendInfo
    int raftPort;
    NacosString site;
    double adWeight;
    double weight;
    NacosString address;
    int failAccessCnt;

    NacosString getIp() const {
        return ip;
    }

    void setIp(const NacosString &ip) {
        NacosInstance::ip = ip;
    }

    int getPort() const {
        return port;
    }

    void setPort(int port) {
        NacosInstance::port = port;
    }

    NacosString getState() const {
        return state;
    }

    void setState(const NacosString &state) {
        NacosInstance::state = state;
    }

    int getRaftPort() const {
        return raftPort;
    }

    void setRaftPort(int raftPort) {
        NacosInstance::raftPort = raftPort;
    }

    NacosString getSite() const {
        return site;
    }

    void setSite(const NacosString &site) {
        NacosInstance::site = site;
    }

    double getAdWeight() const {
        return adWeight;
    }

    void setAdWeight(double adWeight) {
        NacosInstance::adWeight = adWeight;
    }

    double getWeight() const {
        return weight;
    }

    void setWeight(double weight) {
        NacosInstance::weight = weight;
    }

    NacosString getAddress() const {
        return address;
    }

    void setAddress(const NacosString &address) {
        NacosInstance::address = address;
    }

    int getFailAccessCnt() const {
        return failAccessCnt;
    }

    void setFailAccessCnt(int failAccessCnt) {
        NacosInstance::failAccessCnt = failAccessCnt;
    }

    NacosString toString() const{
        return " ip:" + ip + " port:" + NacosStringOps::valueOf(port) +
               " weight:" + NacosStringOps::valueOf(weight) + " adWeight:" + NacosStringOps::valueOf(adWeight) +
               " state:" + state + " failAccessCnt:" + NacosStringOps::valueOf(failAccessCnt);
    }
};
}//namespace nacos

#endif