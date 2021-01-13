//
// Created by liuhanyu on 2020/9/26.
//

#ifndef NACOS_SDK_CPP_UDPLSNR_H_
#define NACOS_SDK_CPP_UDPLSNR_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "src/factory/ObjectConfigData.h"
#include "src/thread/Thread.h"

#define UDP_MSS 64 * 1024

namespace nacos{

typedef struct {
    NacosString type;
    long lastRefTime;
    NacosString data;
} PushPacket;

class UdpNamingServiceListener {
private:
    ObjectConfigData *_objectConfigData;
    volatile bool _started;
    int sockfd;
    int udpReceiverPort;
    struct sockaddr_in cliaddr;
    char receiveBuffer[UDP_MSS];
    Thread *_listenerThread;

    void initializeUdpListener() throw(NacosException);
    static void *listenerThreadFunc(void *param);
public:
    UdpNamingServiceListener(ObjectConfigData *objectConfigData);
    ~UdpNamingServiceListener();
    void start();
    void stop();
};

}//namespace nacos
#endif //NACOS_SDK_CPP_UDPLSNR_H_
