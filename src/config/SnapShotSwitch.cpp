#include "LocalConfigInfoProcessor.h"
#include "SnapShotSwitch.h"

namespace nacos{
bool SnapShotSwitch::getIsSnapShot() {
    return isSnapShot;
};

void SnapShotSwitch::setIsSnapShot(bool isSnapShot) {
    SnapShotSwitch::isSnapShot = isSnapShot;
    //LocalConfigInfoProcessor::cleanAllSnapshot();
};
}//namespace nacos