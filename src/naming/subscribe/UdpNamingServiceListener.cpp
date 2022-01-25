#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "UdpNamingServiceListener.h"
#include "src/config/AppConfigManager.h"
#include "constant/PropertyKeyConst.h"
#include "src/json/JSON.h"
#include "HostReactor.h"
#include "zlib.h"
#include "src/debug/DebugAssertion.h"

#include <iostream>
using namespace std;

namespace nacos {

void UdpNamingServiceListener::initializeUdpListener() NACOS_THROW(NacosException) {
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
    if ( ::bind(sockfd, (const struct sockaddr *)&cliaddr,
              sizeof(cliaddr)) < 0 )
    {
        throw NacosException(NacosException::UNABLE_TO_CREATE_SOCKET, "Unable to bind");
    }

    log_debug("socket bound\n");
}

bool UdpNamingServiceListener::unGzip(char *inBuffer, size_t inSize) {
    //reference:https://zlib.net/zlib_how.html
    /* allocate inflate state */
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    int ret = inflateInit2(&strm, MAX_WBITS + 16);
    if (ret != Z_OK) {
        log_error("failed to perform inflateInit()\n");
        return false;
    }

    strm.avail_in = inSize;
    strm.next_in = (unsigned char*)inBuffer;
    strm.avail_out = sizeof(this->uncompressedData);
    strm.next_out = (unsigned char*)this->uncompressedData;
    ret = inflate(&strm, Z_NO_FLUSH);
    NACOS_ASSERT(ret != Z_STREAM_ERROR);
    switch (ret) {
    case Z_NEED_DICT:
        ret = Z_DATA_ERROR;     /* and fall through */
    case Z_DATA_ERROR:
    case Z_MEM_ERROR:
        (void)inflateEnd(&strm);
        log_error("in switch block, inflate failed with code = %d\n", ret);
        return false;
    }
    
    if (strm.avail_out == 0) {
        log_error("uncompressed data exceeds the size limit, please consider a larger uncompressedData\n");
        return false;
    }

    (void)inflateEnd(&strm);

    this->uncompressedData[sizeof(this->uncompressedData) - strm.avail_out] = '\0';
    return true;
}

void *UdpNamingServiceListener::listenerThreadFunc(void *param) {
    UdpNamingServiceListener *thisObj = (UdpNamingServiceListener*)param;
    log_debug("in thread UdpNamingServiceListener::listenerThreadFunc()\n");
    thisObj->initializeUdpListener();
    while (thisObj->_started) {
        int ret;//also data_len

        log_debug("before recvfrom() socketfd:%d\n", thisObj->sockfd);
        struct sockaddr src_addr = {0};
        socklen_t src_addr_len = sizeof(struct sockaddr_in);
        ret = recvfrom(thisObj->sockfd, (char *)thisObj->receiveBuffer, UDP_MSS, MSG_WAITALL, &src_addr, &src_addr_len);
        log_debug("ret got from recvfrom():%d\n", ret);

        if (ret == -1) {
            if (errno == EINTR) {
                log_debug("got sigint from main thread, exiting...\n");
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
            if (ret <= 2) {
                //the server returns a packet shorter than 2 bytes, which could not be parsed by the listener
                log_warn("got an invalid packet, len = %d, content = %s", ret, thisObj->receiveBuffer);
                continue;
            }
            char *packetToParse = thisObj->receiveBuffer;
            if ((unsigned char)thisObj->receiveBuffer[0] == 0x1f && (unsigned char)thisObj->receiveBuffer[1] == 0x8b) {
                if (!thisObj->unGzip(thisObj->receiveBuffer, ret)) {
                    continue;
                }
                packetToParse = thisObj->uncompressedData;
            }
            pushPacket = JSON::Json2PushPacket(packetToParse);
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
            log_error("error while sending data...%d\n", errno);
        }
    }

    close(thisObj->sockfd);
    return NULL;
}

UdpNamingServiceListener::UdpNamingServiceListener(ObjectConfigData *objectConfigData) {
    _listenerThread = NULL;
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
