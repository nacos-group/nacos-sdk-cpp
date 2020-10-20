#include "init.h"
#include "Debug.h"
#include "src/http/HTTPCli.h"
#include "src/config/LocalConfigInfoProcessor.h"
#include "src/config/SnapShotSwitch.h"
#include "src/config/JVMUtil.h"
#include "utils/UtilAndComs.h"
#include "src/utils/NetUtils.h"
#include "utils/UuidUtils.h"
#include "utils/RandomUtils.h"
static nacos::Init initobj;//Implicitly call the constructors

namespace nacos{
bool Init::inited = false;
bool SnapShotSwitch::isSnapShot = true;
bool JVMUtil::_isMultiInstance = false;

void Init::doInit() {
    if (inited) {
        return;
    }

    inited = true;
    Debug::set_debug_level(DEBUG);
    HTTPCli::HTTP_GLOBAL_INIT();
    LocalConfigInfoProcessor::init();
    UtilAndComs::Init();
    RandomUtils::Init();
    UuidUtils::Init();
    log_debug("LOCAL_FILEROOT_PATH = %s\n", LocalConfigInfoProcessor::LOCAL_FILEROOT_PATH.c_str());
    log_debug("LOCAL_SNAPSHOT_PATH = %s\n", LocalConfigInfoProcessor::LOCAL_SNAPSHOT_PATH.c_str());
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
