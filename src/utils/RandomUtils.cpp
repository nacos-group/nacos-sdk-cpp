#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils/RandomUtils.h"

int RandomUtils::fd;

ThreadLocal<bool> RandomUtils::initedForThisThread(false);

void RandomUtils::Init() {
    fd = open("/dev/urandom", O_RDONLY);
}

void RandomUtils::DeInit() {
    if (fd != 0) {
        close(fd);
        fd = 0;
    }
}

size_t RandomUtils::getRandomBuffer(void *dest, size_t size) {
    size_t bytes_read = 0;
    while (bytes_read < size)
    {
        bytes_read += read(fd, (char*)dest + bytes_read, size - bytes_read);
    }

    return bytes_read;
}

int RandomUtils::random_inner(){
    if (!initedForThisThread.get()){
        srand(time(NULL));
        initedForThisThread.set(true);
    }
    return rand();
}

int RandomUtils::random(int begin, int end) throw (NacosException) {
    //sanity check
    if (begin == end || begin > end) {
        throw NacosException(NacosException::INVALID_PARAM, "end must be greater than begin");
    }
    long offset = random_inner() % (end - begin + 1);
    return begin + offset;
}