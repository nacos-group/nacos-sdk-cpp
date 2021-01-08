#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "UdpNamingServiceListener.h"
#include "src/config/AppConfigManager.h"
#include "constant/PropertyKeyConst.h"
#include "src/json/JSON.h"
#include "HostReactor.h"

#include <iostream>
using namespace std;

namespace nacos {

void UdpNamingServiceListener::initializeUdpListener() throw(NacosException) {
    log_debug("in thread UdpNamingServiceListener::initializeUdpListener()\n");
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        throw NacosException(NacosException::UNABLE_TO_CREATE_SOCKET, "Unable to create socket");
    }

    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling client information
    cliaddr.sin_family = AF_INET; // IPv4
    cliaddr.sin_addr.s_addr = INADDR_ANY;
    cliaddr.sin_port = htons(udpReceiverPort);
    log_debug("udp receiver port = %d\n", cliaddr.sin_port);

    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&cliaddr,
              sizeof(cliaddr)) < 0 )
    {
        throw NacosException(NacosException::UNABLE_TO_CREATE_SOCKET, "Unable to bind");
    }

    log_debug("socket bound\n");
}

void *UdpNamingServiceListener::listenerThreadFunc(void *param) {
    UdpNamingServiceListener *thisObj = (UdpNamingServiceListener*)param;
    log_debug("in thread UdpNamingServiceListener::listenerThreadFunc()\n");
    thisObj->initializeUdpListener();
    while (thisObj->_started) {
        int ret;//also data_len

        log_debug("before recvfrom() socketfd:%d\n", thisObj->sockfd);
        struct sockaddr src_addr;
        socklen_t src_addr_len;
        ret = recvfrom(thisObj->sockfd, (char *)thisObj->receiveBuffer, UDP_MSS, MSG_WAITALL, &src_addr, &src_addr_len);
        log_debug("ret got from recvfrom():%d\n", ret);

        if (ret == -1) {
            if (errno == EINTR) {
                log_debug("got sigint from main thread, exitiing...\n");
                //got kill() signal from main thread, free resources & exit
            }
            //other kinds of error
            break;
        }
        //parse the package
        thisObj->receiveBuffer[ret] = 0;
        log_debug("content got from UDP server is %s\n", thisObj->receiveBuffer);
        PushPacket pushPacket;

        try {
            pushPacket = JSON::Json2PushPacket(thisObj->receiveBuffer);
        } catch (NacosException &e) {
            log_error("Invalid json string got from server:%s\n", thisObj->receiveBuffer);
            continue;
        }

        NacosString ack;

        if (pushPacket.type == "dom" || pushPacket.type == "service") {
            thisObj->_objectConfigData->_hostReactor->processServiceJson(pushPacket.data);

            // send ack to server
            ack = "{\"type\": \"push-ack\", \"lastRefTime\":\"" + NacosStringOps::valueOf(pushPacket.lastRefTime) + "\", \"data\":\"\"}";
        } else if (pushPacket.type == "dump") {
            // dump data to server
            //TODO:Unimplemented
            ack = "{\"type\": \"dump-ack\", \"lastRefTime\": \"" + NacosStringOps::valueOf(pushPacket.lastRefTime) + "\", \"data\":\"\"}";
        } else {
            // do nothing & send ack only
            ack = "{\"type\": \"unknown-ack\", \"lastRefTime\":\"" + NacosStringOps::valueOf(pushPacket.lastRefTime)
                  + "\", \"data\":\"\"}";
        }

        ssize_t recv_ret = sendto(thisObj->sockfd, ack.c_str(), ack.length(), 0, &src_addr, src_addr_len);
        if (recv_ret < 0) {
            log_error("error while sending data...%d", errno);
        }
    }

    close(thisObj->sockfd);
    return NULL;
}

UdpNamingServiceListener::UdpNamingServiceListener(ObjectConfigData *objectConfigData) {
    _started = false;
    _objectConfigData = objectConfigData;
    udpReceiverPort = atoi(_objectConfigData->_appConfigManager->get(PropertyKeyConst::UDP_RECEIVER_PORT).c_str());
    log_debug("udpReceiverPort is %d\n", udpReceiverPort);
    _listenerThread = new Thread(objectConfigData->name + "UDPListener", listenerThreadFunc, (void*)this);
}

void UdpNamingServiceListener::start() {
    if (_started) {
        return;
    }

    _started = true;
    _listenerThread->start();
}

void UdpNamingServiceListener::stop() {
    if (!_started) {
        return;
    }

    _started = false;
    _listenerThread->kill();
    _listenerThread->join();
}

UdpNamingServiceListener::~UdpNamingServiceListener() {
    if (_started) {
        stop();
    }
    if (_listenerThread != NULL) {
        delete _listenerThread;
    }
}

}
