#ifndef __RND_UTILS_H_
#define __RND_UTILS_H_

#include "NacosExceptions.h"
#include "thread/ThreadLocal.h"

/**
 * RandomUtils
 *
 * @author Liu, Hanyu
 * get random buffer from /dev/urandom
 * and helper functions to get random random number
 */

class RandomUtils {
private:
    static int fd;

    static int random_inner();

    static ThreadLocal<bool> initedForThisThread;
public:

    static void Init();

    static void DeInit();

    static size_t getRandomBuffer(void *dest, size_t size);

    /**
     * generates a random number ranges from begin to end (including the begin and the end)
     *
     * @param begin begin of the range (inclusive)
     * @param end   end of the range   (inclusive)
     * @return a long random number
     * @throw (NacosException) if begin >= end
     */
    static int random(int begin, int end) throw (NacosException);
};

#endif
