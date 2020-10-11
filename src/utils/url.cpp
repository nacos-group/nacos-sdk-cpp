#include "utils/url.h"
#include <curl/curl.h>

namespace nacos{
NacosString urlencode(const NacosString &content) {
    NacosString result;
    CURL *curl = curl_easy_init();
    char *output = NULL;
    if (curl) {
        output = curl_easy_escape(curl, content.c_str(), content.length());
    }

    if (output) {
        result = output;
        curl_free(output);
    }

    curl_easy_cleanup(curl);
    return result;
}

NacosString urldecode(const NacosString &content) {
    NacosString result;
    CURL *curl = curl_easy_init();
    char *output = NULL;
    if (curl) {
        output = curl_easy_unescape(curl, content.c_str(), content.length(), NULL);
    }

    if (output) {
        result = output;
        curl_free(output);
    }

    curl_easy_cleanup(curl);
    return result;
}
}//namespace nacos
