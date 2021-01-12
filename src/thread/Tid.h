#ifndef __TID_HELPER_H_
#define __TID_HELPER_H_
#include <sys/syscall.h>
#include <unistd.h>

#if defined(__CYGWIN__) || defined(MS_WINDOWS)
//TODO:for windows/cygwin
#elif defined(linux)

//for linux
#define gettidv1() syscall(__NR_gettid)
//#define gettidv2() syscall(SYS_gettid)

#elif defined(FreeBSD)

//Mac OS code goes here
#define pid_t uint64_t
pid_t gettidv1() {
    pid_t tid;
    pthread_threadid_np(NULL, &tid);

    return tid;
}


#endif

#endif
