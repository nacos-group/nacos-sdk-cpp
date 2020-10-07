#ifndef __RND_UTILS_H_
#define __RND_UTILS_H_

/**
 * RandomUtils
 *
 * @author Liu, Hanyu
 * Generates random number from /dev/urandom
 * And various helper functions
 */

class RandomUtils {
private:
    static int fd;
public:

    static void Init();

    static void DeInit();

    static size_t getRandomBuffer(void *dest, size_t size);
};

#endif
