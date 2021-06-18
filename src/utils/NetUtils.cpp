#include "NetUtils.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <ifaddrs.h>
#include <src/log/Logger.h>
#include <string.h>
#include <unistd.h>

#define HOST_AND_LEN 250

namespace nacos{

NacosString NetUtils::getHostIp() NACOS_THROW(NacosException){

    struct ifaddrs *ifaddr, *ifa;
    int s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1)
    {
        throw NacosException(NacosException::UNABLE_TO_GET_HOST_IP, "Failed to get IF address");
    }


    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        log_debug("iterating on iface=%s\n", ifa->ifa_name);
        if (ifa->ifa_addr == NULL || !(ifa->ifa_addr->sa_family==AF_INET)) {
            continue;
        }

        if((strcmp(ifa->ifa_name,"lo")==0)) {
            continue;
        }

        s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        if (s != 0) {
            freeifaddrs(ifaddr);
            throw NacosException(NacosException::UNABLE_TO_GET_HOST_IP, "Failed to get IF address");
        }

        log_debug("selected iface=%s ip=%s\n", ifa->ifa_name, host);
        freeifaddrs(ifaddr);
        return host;
    }
    //Usually the program will not run to here
    throw NacosException(NacosException::UNABLE_TO_GET_HOST_IP, "Failed to get IF address");
}

NacosString NetUtils::getHostName() NACOS_THROW(NacosException)
{
    char hostname[HOST_AND_LEN];
    
    int res = gethostname(hostname, HOST_AND_LEN);
    if (res == 0) {
        return NacosString(hostname);
    }

    throw NacosException(NacosException::UNABLE_TO_GET_HOST_NAME, "Failed to get hostname, errno = " + NacosStringOps::valueOf(errno));
}

}//namespace nacos
