#include <stddef.h>
#include "src/utils/TimeUtils.h"

namespace nacos{
int64_t TimeUtils::getCurrentTimeInMs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void TimeUtils::getCurrentTimeInStruct(struct timeval &tv) {
    gettimeofday(&tv, NULL);
}

}//namespace nacos
