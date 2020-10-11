#ifndef NACOS_SDK_CPP_SERVERSELECTOR_H
#define NACOS_SDK_CPP_SERVERSELECTOR_H

#include "NacosServerInfo.h"
#include <list>

/*
* ServerSelector
* Author: Liu, Hanyu
* Selector interface, to select one server from the select list
* User can implement different selecting strategies (e.g.: Weighed, Random, Least used) by implementing select() method
*/
namespace nacos{
class ServerSelector {
public:
    virtual NacosServerInfo select(std::list <NacosServerInfo> &serverList) = 0;

    virtual ~ServerSelector() {};
};
}//namespace nacos

#endif //NACOS_SDK_CPP_SERVERSELECTOR_H
