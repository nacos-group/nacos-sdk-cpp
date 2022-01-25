#include "factory/NacosFactoryFactory.h"
#include "src/factory/NacosServiceFactory.h"
namespace nacos {

INacosServiceFactory *NacosFactoryFactory::getNacosFactory(const NacosString &_configFile) {
        NacosServiceFactory *factory = new NacosServiceFactory(_configFile);
        return factory;
}

INacosServiceFactory *NacosFactoryFactory::getNacosFactory(Properties &_props) {
        NacosServiceFactory *factory = new NacosServiceFactory(_props);
        return factory;
}

}