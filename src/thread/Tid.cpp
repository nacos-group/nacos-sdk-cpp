#include "Tid.h"

#if defined(__CYGWIN__) || defined(MS_WINDOWS)
//TODO:for windows/cygwin

#elif defined(linux)
//for linux
//solved in header file

#elif defined(FreeBSD)

//Mac OS code goes here
namespace nacos {
pid_t gettidv1() {
    pid_t tid;
    pthread_threadid_np(NULL, &tid);

    return tid;
}
}

#endif//OS-specific code
