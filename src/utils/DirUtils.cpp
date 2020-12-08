#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "utils/DirUtils.h"

#if defined(__CYGWIN__) || defined(MS_WINDOWS)
#define PATH_MAX 260
#else
#include <linux/limits.h>
#endif


namespace nacos{
NacosString DirUtils::getHome() {
    struct passwd *pw = getpwuid(getuid());
    NacosString homedir = pw->pw_dir;
    return homedir;
}

NacosString DirUtils::getCwd() {
    char cwd[PATH_MAX];
    NacosString cwds;
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        cwds = cwd;
        return cwds;
    }

    return NULLSTR;
}
}//namespace nacos
