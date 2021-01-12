#ifndef __TID_HELPER_H_
#define __TID_HELPER_H_
#include <sys/syscall.h>
#include <unistd.h>
#define gettidv1() syscall(__NR_gettid)
//#define gettidv2() syscall(SYS_gettid)
#endif