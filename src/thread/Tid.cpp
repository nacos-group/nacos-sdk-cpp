#include "src/thread/Tid.h"

#if defined(__CYGWIN__) || defined(MS_WINDOWS)
//TODO:for windows/cygwin

#elif defined(linux)
//for linux
//solved in header file

#elif defined(__APPLE__) && defined(__MACH__)
//Mac OS code goes here
#include<pthread.h>
TID_T gettidv1() {
    TID_T tid;
    pthread_threadid_np(NULL, &tid);

    return tid;
}
#else
//regard the system as an unix-like system
//for linux solved in header file

#endif//OS-specific code
