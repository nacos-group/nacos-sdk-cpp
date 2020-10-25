#ifndef __INIT_H_
#define __INIT_H_

namespace nacos{
class Init {
private:
    static bool inited;
public:
    Init() { doInit(); };

    ~Init() { doDeinit(); };

    static void doInit();

    static void doDeinit();
};
}//namespace nacos

#endif