#include <stdlib.h>

const char* getEnv(const char* env) {
    if (env == NULL) {
        return NULL;
    }
    char* var = getenv(env);
    return var;
}