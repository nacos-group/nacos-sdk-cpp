<img src="https://github.com/alibaba/nacos/blob/develop/doc/Nacos_Logo.png" width="50%" height="50%" />

# Nacos-sdk-cpp

Nacos-sdk-cpp for c++ clients allow users to access Nacos service,it's support service discovery and dynamic configuration.


[![Gitter](https://badges.gitter.im/alibaba/nacos.svg)](https://gitter.im/alibaba/nacos?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)   [![License](https://img.shields.io/badge/license-Apache%202-4EB1BA.svg)](https://www.apache.org/licenses/LICENSE-2.0.html)
[![Gitter](https://travis-ci.org/alibaba/nacos.svg?branch=master)](https://travis-ci.org/alibaba/nacos)

# Quick Examples
## Configuration

### Get Config
```C++
#include <iostream>
#include "factory/NacosServiceFactory.h"
#include "PropertyKeyConst.h"
#include "DebugAssertion.h"
#include "ResourceGuard.h"
#include "Debug.h"

using namespace std;

int main() {
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";//Server address
    NacosServiceFactory *factory = new NacosServiceFactory(props);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);
    NacosString ss = "";
    try {
        ss = n->getConfig("k", NULLSTR, 1000);
    }
    catch (NacosException &e) {
        cout <<
             "Request failed with curl code:" << e.errorcode() << endl <<
             "Reason:" << e.what() << endl;
        return -1;
    }
    cout << ss << endl;

    return 0;
}
``` 

### Set Config

```C++
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "factory/NacosServiceFactory.h"
#include "ResourceGuard.h"
#include "PropertyKeyConst.h"
#include "DebugAssertion.h"
#include "Debug.h"

using namespace std;

int main() {
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";//server address
    NacosServiceFactory *factory = new NacosServiceFactory(props);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);
    bool bSucc = false;
    NacosString ss = "";

    try {
        bSucc = n->publishConfig("Cfg_key", NULLSTR, "Cfg_val");
        int retry = 0;
        ss = n->getConfig("Cfg_key", NULLSTR, 1000);
        while (!(ss == "Cfg_val") && retry++ < 10) {
            ss = n->getConfig("Cfg_key", NULLSTR, 1000);
        }

        if (!(ss == "Cfg_val")) {
            throw NacosException(0, "getConfig() failed.");
        }
    }
    catch (NacosException &e) {
        cout <<
             "Request failed with curl code:" << e.errorcode() << endl <<
             "Reason:" << e.what() << endl;

        return -1;
    }
    cout << "Publishing Key:Cfg_key with value:Cfg_val result:" << bSucc << endl;

    return 0;
}
``` 

### Listen to key change & Cancel listening

```C++
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "factory/NacosServiceFactory.h"
#include "ResourceGuard.h"
#include "listen/Listener.h"
#include "PropertyKeyConst.h"
#include "DebugAssertion.h"
#include "Debug.h"

using namespace std;

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
    NacosServiceFactory *factory = new NacosServiceFactory(props);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);

    MyListener *theListener = new MyListener(1);//You don't need to free it, since it will be deleted by the function removeListener
    n->addListener("dqid", NULLSTR, theListener);//All changes on the key dqid will be received by MyListener

    cout << "Input a character to continue" << endl;
    getchar();
    cout << "remove listener" << endl;
    n->removeListener("dqid", NULLSTR, theListener);//Cancel listening
    getchar();

    return 0;
}
```

## Naming

### Register Instance & Unregister Instance

```C++
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "naming/NamingProxy.h"
#include "naming/NacosNamingService.h"
#include "factory/NacosServiceFactory.h"
#include "ResourceGuard.h"
#include "naming/Instance.h"
#include "Constants.h"
#include "utils/UtilAndComs.h"
#include "http/HTTPCli.h"
#include "DebugAssertion.h"
#include "Debug.h"
#include "NacosString.h"
#include "Properties.h"
#include "PropertyKeyConst.h"

using namespace std;

int main() {
    Properties configProps;
    configProps[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1";
    NacosServiceFactory *factory = new NacosServiceFactory(configProps);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    NamingService *namingSvc = factory->CreateNamingService();
    ResourceGuard <NamingService> _serviceFactory(namingSvc);
    Instance instance;
    instance.clusterName = "DefaultCluster";
    instance.ip = "127.0.0.1";
    instance.port = 2333;
    instance.instanceId = "1";
    instance.ephemeral = true;

    //Registers 5 services named TestNamingService1...5
    try {
        for (int i = 0; i < 5; i++) {
            NacosString serviceName = "TestNamingService" + NacosStringOps::valueOf(i);
            instance.port = 2000 + i;
            namingSvc->registerInstance(serviceName, instance);
        }
    }
    catch (NacosException &e) {
        cout << "encounter exception while registering service instance, raison:" << e.what() << endl;
        return -1;
    }
    sleep(30);
    try {
        for (int i = 0; i < 5; i++) {
            NacosString serviceName = "TestNamingService" + NacosStringOps::valueOf(i);

            namingSvc->deregisterInstance(serviceName, "127.0.0.1", 2000 + i);
            sleep(1);
        }
    }
    catch (NacosException &e) {
        cout << "encounter exception while registering service instance, raison:" << e.what() << endl;
        return -1;
    }
    sleep(30);

    return 0;
}
```

### Subscribe & Unsubscribe

```C++
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "factory/NacosServiceFactory.h"
#include "ResourceGuard.h"
#include "naming/subscribe/EventListener.h"
#include "PropertyKeyConst.h"
#include "DebugAssertion.h"
#include "Debug.h"

using namespace std;

class MyServiceListener : public EventListener {
private:
    int num;
public:
    MyServiceListener(int num) {
        this->num = num;
    }

    void receiveNamingInfo(const ServiceInfo &serviceInfo){
        cout << "===================================" << endl;
        cout << "Watcher: " << num << endl;
        cout << "Watched service UPDATED: " << serviceInfo.toInstanceString() << endl;
        cout << "===================================" << endl;

    }
};

int main() {
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
    //Interval for poller to check the status of subscribed services(unit:Ms), 30000 by default
    //Here we set it to 5000 to see the output more quick
    props[PropertyKeyConst::TCP_NAMING_POLL_INTERVAL] = "5000";
    NacosServiceFactory *factory = new NacosServiceFactory(props);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    NamingService *n = factory->CreateNamingService();
    ResourceGuard <NamingService> _serviceFactory(n);

    n->subscribe("ss", new MyServiceListener(1));
    cout << "Press any key to register services" << endl;
    getchar();

    n->registerInstance("ss", "127.0.0.1", 33);
    n->registerInstance("ss", "127.0.0.1", 34);
    cout << "Press any key to deregister services" << endl;
    getchar();

    n->deregisterInstance("ss", "127.0.0.1", 33);
    n->deregisterInstance("ss", "127.0.0.1", 34);
    cout << "All instances Unregistered, press any key to finish testing" << endl;
    getchar();

    return 0;
}
```

### Get all instances of a service

```C++
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include "naming/NamingProxy.h"
#include "factory/NacosServiceFactory.h"
#include "naming/Instance.h"
#include "Constants.h"
#include "utils/UtilAndComs.h"
#include "http/HTTPCli.h"
#include "DebugAssertion.h"
#include "Debug.h"
#include "NacosString.h"
#include "Properties.h"
#include "PropertyKeyConst.h"
#include "ResourceGuard.h"

using namespace std;

int main() {
    Properties configProps;
    configProps[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1";
    NacosServiceFactory *factory = new NacosServiceFactory(configProps);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    NamingService *namingSvc = factory->CreateNamingService();
    ResourceGuard <NamingService> _guardService(namingSvc);

    list <Instance> instances = namingSvc->getAllInstances("TestNamingService1");
    cout << "getAllInstances from server:" << endl;
    for (list<Instance>::iterator it = instances.begin();
         it != instances.end(); it++) {
        cout << "Instance:" << it->toString() << endl;
    }

    return 0;
}
```

# About Nacos

Nacos (official site: [http://nacos.io](http://nacos.io)) is an easy-to-use platform designed for dynamic service discovery and configuration and service management. It helps you to build cloud native applications and microservices platform easily.

Service is a first-class citizen in Nacos. Nacos supports almost all type of services，for example，[Dubbo/gRPC service](https://nacos.io/en-us/docs/use-nacos-with-dubbo.html)、[Spring Cloud RESTFul service](https://nacos.io/en-us/docs/use-nacos-with-springcloud.html) or [Kubernetes service](https://nacos.io/en-us/docs/use-nacos-with-kubernetes.html).
