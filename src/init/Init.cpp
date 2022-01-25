#include "Init.h"
#include "src/http/HTTPCli.h"
#include "src/config/SnapShotSwitch.h"
#include "src/config/JVMUtil.h"
#include "naming/ServiceInfo2.h"
#include "constant/UtilAndComs.h"
#include "src/utils/UuidUtils.h"
#include "src/utils/RandomUtils.h"
#include "src/thread/Thread.h"
#include "src/crypto/MACProvider.h"
static nacos::Init initobj;//Implicitly call the constructors

namespace nacos{
Mutex Init::initflagMutex;
bool Init::inited = false;
bool SnapShotSwitch::isSnapShot = true;
bool JVMUtil::_isMultiInstance = false;
ServiceInfo2 ServiceInfo2::nullServiceInfo2;

void Init::doInit() {
    if (inited) {
        return;
    }
    {
        LockGuard _initGuard(initflagMutex);
        if (inited) {
            return;
        }

        Logger::Init();
        MACProvider::Init();
        HTTPCli::HTTP_GLOBAL_INIT();
        UtilAndComs::Init();
        RandomUtils::Init();
        UuidUtils::Init();
        Thread::Init();
        ServiceInfo2::nullServiceInfo2.setNull(true);
        inited = true;
    }
}

void Init::doDeinit() {
    if (!inited) {
        return;
    }

    {
        LockGuard _initGuard(initflagMutex);
        if (!inited) {
            return;
        }

        MACProvider::DeInit();
        Thread::DeInit();
        UuidUtils::DeInit();
        RandomUtils::DeInit();
        HTTPCli::HTTP_GLOBAL_DEINIT();
        Logger::deInit();
        inited = false;
    }
}

}//namespace nacos
