#include <stddef.h>
#include "TimeUtils.h"

namespace nacos{
int64_t TimeUtils::getCurrentTimeInMs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

}//namespace nacos
