#ifndef __SS_SWITCH_H_
#define __SS_SWITCH_H_

namespace nacos{
class SnapShotSwitch {
private:
    /**
     * whether use local cache
     */
    static bool isSnapShot;
public:
    static bool getIsSnapShot();

    static void setIsSnapShot(bool isSnapShot);
};
}//namespace nacos

#endif