#ifndef __TIME_UTILS_H_
#define __TIME_UTILS_H_

#include <stdint.h>
#include <sys/time.h>

namespace nacos{
class TimeUtils {
public:
    static int64_t getCurrentTimeInMs();
    static void getCurrentTimeInStruct(struct timeval &tv);

};
}//namespace nacos

#endif
