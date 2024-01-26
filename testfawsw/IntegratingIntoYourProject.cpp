#include "factory/NacosServiceFactory.h"

#include <iostream>
#include <unistd.h>
#include "Nacos.h"
#include "log/Logger.h"

using namespace std;
using namespace nacos;

class MyListener : public Listener {
private:
    int num;
public:
    MyListener(int num) {
        this->num = num;
    }

    void receiveConfigInfo(const NacosString &configInfo) {
        cout << "===================================" << endl;
        cout << "Watcher" << num << endl;
        cout << "Watched Key UPDATED:" << configInfo << endl;
        cout << "===================================" << endl;
    }
};

int main() {
    Logger::setLogLevel(DEBUG);
    Properties configProps;
    // configProps[PropertyKeyConst::SERVER_ADDR] = "";
    // configProps[PropertyKeyConst::AUTH_USERNAME] = "test";
    // configProps[PropertyKeyConst::AUTH_PASSWORD] = "test";
    // configProps[PropertyKeyConst::NAMESPACE] = "dev";
    // configProps[PropertyKeyConst::SERVER_ADDR] = "";
    // configProps[PropertyKeyConst::AUTH_USERNAME] = "hqzl3";
    // configProps[PropertyKeyConst::AUTH_PASSWORD] = "sitHqzl3nacos2022";
    // configProps[PropertyKeyConst::NAMESPACE] = "fc2-faw-szhb-hqzl3";
    NacosServiceFactory *factory = new NacosServiceFactory(configProps);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);


    // NamingService *namingSvc = factory->CreateNamingService();
    // ResourceGuard <NamingService> _serviceFactory(namingSvc);
    // Instance instance;
    // instance.clusterName = "DefaultCluster";
    // instance.ip = "127.0.0.1";
    // instance.port = 2333;
    // instance.instanceId = "1";
    // instance.ephemeral = true;

    //Registers 5 services named TestNamingService1...5
    // try {
    //     for (int i = 0; i < 5; i++) {
    //         NacosString serviceName = "TestNamingService" + NacosStringOps::valueOf(i);
    //         instance.port = 2000 + i;
    //         namingSvc->registerInstance(serviceName, instance);
    //     }
    // }
    // catch (NacosException &e) {
    //     cout << "encounter exception while registering service instance, raison:" << e.what() << endl;
    //     return -1;
    // }
    // sleep(30);
    // try {
    //     for (int i = 0; i < 5; i++) {
    //         NacosString serviceName = "TestNamingService" + NacosStringOps::valueOf(i);

    //         namingSvc->deregisterInstance(serviceName, "127.0.0.1", 2000 + i);
    //         sleep(1);
    //     }
    // }
    // catch (NacosException &e) {
    //     cout << "encounter exception while registering service instance, raison:" << e.what() << endl;
    //     return -1;
    // }
    // sleep(30);

    // return 0;

    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);

    MyListener *theListener = new MyListener(1);//You don't need to free it, since it will be deleted by the function removeListener
    n->addListener("tts-script.yaml", "nlu", theListener);//All changes on the key dqid will be received by MyListener

    cout << "Input a character to continue" << endl;
    getchar();
    cout << "remove listener" << endl;
    n->removeListener("tts-script.yaml", "nlu", theListener);//Cancel listening
    getchar();

    return 0;

    // NacosString ss = "";
    // try {
    //     ss = n->getConfig("tts-script.yaml", "nlu", 10000);
    // }
    // catch (NacosException &e) {
    //     cout <<
    //          "Request failed with curl code:" << e.errorcode() << endl <<
    //          "Reason:" << e.what() << endl;
    //     return -1;
    // }
    // cout << ss << endl;

    // return 0;
}