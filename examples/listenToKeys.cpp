#include <iostream>
#include "Nacos.h"
#include <stdio.h>

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
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
    INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(props);
    ResourceGuard <INacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);

    MyListener *theListener = new MyListener(1);//You don't need to free it, since it will be deleted by the function removeListener
    n->addListener("dqid", NULLSTR, theListener);//All changes on the key dqid will be received by MyListener

    cout << "Input a character to continue" << endl;
    getchar();
    cout << "remove listener" << endl;
    n->removeListener("dqid", NULLSTR, theListener);//Cancel listening
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();

    return 0;
}
