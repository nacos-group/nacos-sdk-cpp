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
#include "Compatibility.h"

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
    //assume the max compress ratio = 90%
    char uncompressedData[UDP_MSS * 10];
    Thread *_listenerThread;

    void initializeUdpListener() NACOS_THROW(NacosException);
    static void *listenerThreadFunc(void *param);
    bool unGzip(char *inBuffer, size_t inSize);
public:
    UdpNamingServiceListener(ObjectConfigData *objectConfigData);
    ~UdpNamingServiceListener();
    void start();
    void stop();
};

}//namespace nacos
#endif //NACOS_SDK_CPP_UDPLSNR_H_
