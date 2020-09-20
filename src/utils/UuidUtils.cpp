#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include "utils/UuidUtils.h"

int UuidUtils::fd;

void UuidUtils::Init() {
    fd = open("/dev/urandom", O_RDONLY);
}

void UuidUtils::DeInit() {
    if (fd != 0) {
        close(fd);
        fd = 0;
    }
}

NacosString UuidUtils::generateUuid() {
    char random_num[UUID_LEN_BYTES];
    char str_buffer[33];


    size_t bytes_read = 0;
    while (bytes_read < UUID_LEN_BYTES)//an UUID has 128 bits(16 bytes), so we need to 16 bytes from the urandom
    {
        bytes_read += read(fd, &random_num[bytes_read], UUID_LEN_BYTES - bytes_read);
    }

    int32_t *val_ptr = (int32_t *) random_num;
    sprintf(str_buffer, "%08X%08X%08X%08X", val_ptr[0], val_ptr[1], val_ptr[2], val_ptr[3]);

    NacosString uuid = str_buffer;
    return uuid;
}
