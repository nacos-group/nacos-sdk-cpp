#ifndef __TID_HELPER_H_
#define __TID_HELPER_H_

#if defined(__CYGWIN__) || defined(MS_WINDOWS)
//TODO:for windows/cygwin

#elif defined(linux)
//for linux

#include <sys/syscall.h>
#include <unistd.h>
#define gettidv1() syscall(__NR_gettid)
//#define gettidv2() syscall(SYS_gettid)

#elif defined(FreeBSD)
//Mac OS code goes here

#define pid_t uint64_t

#endif//OS-specific code

#endif//HEADER_GUARD