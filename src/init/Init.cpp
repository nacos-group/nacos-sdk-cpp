#include "Init.h"
#include "src/http/HTTPCli.h"
#include "src/config/SnapShotSwitch.h"
#include "src/config/JVMUtil.h"
#include "naming/ServiceInfo2.h"
#include "utils/UtilAndComs.h"
#include "utils/UuidUtils.h"
#include "utils/RandomUtils.h"
static nacos::Init initobj;//Implicitly call the constructors

namespace nacos{
bool Init::inited = false;
bool SnapShotSwitch::isSnapShot = true;
bool JVMUtil::_isMultiInstance = false;
ServiceInfo2 ServiceInfo2::nullServiceInfo2;

void Init::doInit() {
    if (inited) {
        return;
    }

    inited = true;
    Debug::set_debug_level(ERROR);
    HTTPCli::HTTP_GLOBAL_INIT();
    UtilAndComs::Init();
    RandomUtils::Init();
    UuidUtils::Init();
    ServiceInfo2::nullServiceInfo2.setNull(true);
}

void Init::doDeinit() {
    if (!inited) {
        return;
    }
    inited = false;
    UuidUtils::DeInit();
    RandomUtils::DeInit();
    HTTPCli::HTTP_GLOBAL_DEINIT();
}

}//namespace nacos
