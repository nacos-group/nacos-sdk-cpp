#ifndef __TID_HELPER_H_
#define __TID_HELPER_H_
#if defined(__CYGWIN__) || defined(MS_WINDOWS)
//TODO:for windows/cygwin
#include <sys/syscall.h>
#include <unistd.h>
#define TID_T pid_t
#define gettidv1() syscall(__NR_gettid)

#elif defined(linux) || defined(LINUX)
//for linux
#include <sys/syscall.h>
#include <unistd.h>
#define TID_T pid_t
#define gettidv1() syscall(__NR_gettid)
//#define gettidv2() syscall(SYS_gettid)

#elif defined(__APPLE__) && defined(__MACH__)
//Mac OS code goes here
#define TID_T unsigned long long
TID_T gettidv1();

#else
//regard the system as an unix-like system
#include <sys/syscall.h>
#include <unistd.h>
#define TID_T pid_t
#define gettidv1() syscall(__NR_gettid)

#endif//OS-specific code

#endif//HEADER_GUARD
