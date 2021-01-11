#ifndef __INIT_H_
#define __INIT_H_

#include "src/thread/Mutex.h"

namespace nacos{
class Init {
private:
    static bool inited;
    static Mutex initflagMutex;
public:
    Init() {};

    ~Init() { doDeinit(); };

    static void doInit();

    static void doDeinit();
};
}//namespace nacos

#endif