#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include "utils/RandomUtils.h"

int RandomUtils::fd;

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