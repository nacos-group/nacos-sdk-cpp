#ifndef __INIT_H_
#define __INIT_H_

namespace nacos{
class Init {
public:
    Init() {};

    ~Init() {};

    static void doInit();

    static void doDeinit();
};
}//namespace nacos

#endif