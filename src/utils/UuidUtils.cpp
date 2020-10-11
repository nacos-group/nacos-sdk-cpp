#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include "utils/UuidUtils.h"
#include "utils/RandomUtils.h"

namespace nacos{
void UuidUtils::Init() {
}

void UuidUtils::DeInit() {
}

NacosString UuidUtils::generateUuid() {
    char random_num[UUID_LEN_BYTES];
    char str_buffer[33];

    //an UUID has 128 bits(16 bytes), so we need to get 16 bytes from the urandom
    RandomUtils::getRandomBuffer(random_num, UUID_LEN_BYTES);

    int32_t *val_ptr = (int32_t *) random_num;
    sprintf(str_buffer, "%08X%08X%08X%08X", val_ptr[0], val_ptr[1], val_ptr[2], val_ptr[3]);

    NacosString uuid = str_buffer;
    return uuid;
}

}//namespace nacos
