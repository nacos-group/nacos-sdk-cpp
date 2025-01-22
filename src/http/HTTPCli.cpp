#include <string.h>
#include "HTTPCli.h"
#include "src/utils/url.h"
#include "constant/UtilAndComs.h"
#include "src/log/Logger.h"

using namespace std;

namespace nacos{
static size_t
receiveResponseCallback(
        void *contents,
        size_t size,
        size_t nmemb,
        void *userp
) {
    size_t realsize = size * nmemb;
    NacosString *strbuf = (NacosString *) userp;

    strbuf->append((char *) contents, realsize);

    return realsize;
}

static size_t receiveHeaderCallback(
        void *contents,
        size_t size,
        size_t nmemb,
        void *userp
) {
    char *content_s = (char *) contents;
    //Parse the 'HeaderName: HeaderContent' format
    char *pos = strchr(content_s, ':');
    if (pos != NULL)//Skip status
    {
        std::map <NacosString, NacosString> *respheaders = (std::map <NacosString, NacosString> *) userp;
        NacosString k = NacosString(content_s, pos - content_s);
        NacosString v = NacosString(pos + 1);
        (*respheaders)[k] = v;
    }
    size_t realsize = size * nmemb;

    log_debug("[HTTPCli]-receivedHeaders: %s", (char *) contents);

    return realsize;
}

HTTPCli::HTTPCli() {
    /* init the curl session */
    pthread_key_create(&pthreadKey, HTTPCli::destroyCurlHandle);
}

CURL *HTTPCli::getCurlHandle() {
    CURL *curlHandle = pthread_getspecific(pthreadKey);
    if (curlHandle == NULL) {
        curlHandle = curl_easy_init();
        pthread_setspecific(pthreadKey, reinterpret_cast<void *>(curlHandle));
    }
    return curlHandle;
}

void HTTPCli::destroyCurlHandle(void *arg) {
    CURL *curlHandle = reinterpret_cast<CURL *>(arg);
    if (curlHandle != NULL) {
        curl_easy_cleanup(curlHandle);
        curlHandle = NULL;
    }
}

void HTTPCli::HTTPBasicSettings(CURL *curlHandle) {
    curl_easy_setopt(curlHandle, CURLOPT_USERAGENT, UtilAndComs::VERSION.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, receiveResponseCallback);
    curl_easy_setopt(curlHandle, CURLOPT_HEADERFUNCTION, receiveHeaderCallback);

    curl_easy_setopt(curlHandle, CURLOPT_TCP_KEEPALIVE, 1L);

    /* keep-alive idle time to 120 seconds */
    curl_easy_setopt(curlHandle, CURLOPT_TCP_KEEPIDLE, 120L);

    /* interval time between keep-alive probes: 60 seconds */
    curl_easy_setopt(curlHandle, CURLOPT_TCP_KEEPINTVL, 60L);
    curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, 1);

}

NacosString HTTPCli::encodingParams(list <NacosString> &params) {
    NacosString encodedParms = "";
    for (list<NacosString>::iterator it = params.begin(); it != params.end(); it++) {
        if (encodedParms.compare("") != 0) {
            encodedParms.append("&");
        }
        encodedParms.append(urlencode(*it));
        it++;
        encodedParms.append("=" + urlencode(*it));
    }

    return encodedParms;
}

NacosString HTTPCli::encodingParams(map <NacosString, NacosString> &params) {
    NacosString encodedParms = "";
    for (map<NacosString, NacosString>::iterator it = params.begin(); it != params.end(); it++) {
        if (encodedParms.compare("") != 0) {
            encodedParms.append("&");
        }
        encodedParms.append(it->first + "=" + it->second);
    }

    return encodedParms;
}

void HTTPCli::assembleHeaders(list <NacosString> &assembledHeaders, list <NacosString> &headers) {
    for (list<NacosString>::iterator it = headers.begin(); it != headers.end(); it++) {
        NacosString curHeader = "";
        curHeader.append(*it);
        curHeader += ": ";
        it++;
        curHeader.append(*it);
        assembledHeaders.push_back(curHeader);
    }
}

HttpResult HTTPCli::httpGet
        (
                const NacosString &path,
                list <NacosString> &headers,
                list <NacosString> &paramValues,
                const NacosString &encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    NacosString parmVal;
    parmVal = encodingParams(paramValues);
    return httpGetInternal(path, headers, parmVal, encoding, readTimeoutMs);
}

HttpResult HTTPCli::httpGet
        (
                const NacosString &path,
                list <NacosString> &headers,
                map <NacosString, NacosString> &paramValues,
                const NacosString &encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    NacosString parmVal;
    parmVal = encodingParams(paramValues);
    return httpGetInternal(path, headers, parmVal, encoding, readTimeoutMs);
}

HttpResult HTTPCli::httpGetInternal
        (
                const NacosString &path,
                list <NacosString> &headers,
                const NacosString &paramValues,
                const NacosString &encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    CURL *curlHandle = getCurlHandle();
    CURLcode curlres;

    NacosString Url = path;

    if (paramValues.compare("") != 0) {
        Url += "?" + paramValues;
    }
    log_debug("[HTTPCli]-Get:Assembled URL with parms:%s\n", Url.c_str());

    /*Headers look like:
        foo
        bar
        bax
        lol
    We convert it into sth like with assembleHeaders():
        foo: bar
        bax: lol
    */
    list <NacosString> assembledHeaders;
    assembleHeaders(assembledHeaders, headers);

    //clear-ups
    curl_easy_reset(curlHandle);
    /* specify URL to get */
    curl_easy_setopt(curlHandle, CURLOPT_URL, Url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, 1);

    //Setup common parameters
    HTTPBasicSettings(curlHandle);
    /* send all data to this function  */
    NacosString strbuf = "";
    /* we pass our 'strbuf' struct to the callback function */
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *) &strbuf);

    /* Get response headers from the response */
    std::map <NacosString, NacosString> respheaders;
    curl_easy_setopt(curlHandle, CURLOPT_HEADERDATA, (void *) &respheaders);

    //TODO:Time out in a more precise way
    curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT, readTimeoutMs / 1000);

    /*Add the request headers to the request*/
    struct curl_slist *headerlist = NULL;

    for (list<NacosString>::iterator it = assembledHeaders.begin(); it != assembledHeaders.end(); it++) {
        headerlist = curl_slist_append(headerlist, it->c_str());
    }

    if (headerlist != NULL) {
        curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headerlist);
    }


    /* get it! */
    curlres = curl_easy_perform(curlHandle);

    /*Since the headerlist is not needed anymore, free it to prevent mem leak*/
    if (headerlist != NULL) {
        curl_slist_free_all(headerlist);
        headerlist = NULL;
    }

    if (curlres != CURLE_OK) {
        log_error("[HTTPCli]-Get:curl_easy_perform() failed: %d - %s\n",
                  curlres, curl_easy_strerror(curlres));
        throw NetworkException(curlres, curl_easy_strerror(curlres));
    }

    long response_code;
    curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &response_code);
    HttpResult httpresp = HttpResult(response_code, strbuf, respheaders);
    httpresp.curlcode = curlres;
    log_debug("[HTTPCli]-Get:%lu bytes retrieved\n", (unsigned long) strbuf.length());
    log_debug("[HTTPCli]-Get:content:%s\n", strbuf.c_str());
    log_debug("[HTTPCli]-Get:resp-code:%d\n", response_code);

    return httpresp;
}

/*httpPost, post data are passed in list form like this:
	foo
	bar
	bax
	lol
We convert it into sth like this:
	foo=bar&bax=lol
*/
HttpResult HTTPCli::httpPost
        (
                const NacosString &path,
                list <NacosString> &headers,
                list <NacosString> &paramValues,
                const NacosString &encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    NacosString parmVal;
    parmVal = encodingParams(paramValues);
    return httpPostInternal(path, headers, parmVal, encoding, readTimeoutMs);
}

//httpPost, post data are passed in map form
HttpResult HTTPCli::httpPost
        (
                const NacosString &path,
                list <NacosString> &headers,
                map <NacosString, NacosString> &paramValues,
                const NacosString &encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    NacosString parmVal;
    parmVal = encodingParams(paramValues);
    return httpPostInternal(path, headers, parmVal, encoding, readTimeoutMs);
}

//Implement of httpPost
HttpResult HTTPCli::httpPostInternal
        (
                const NacosString &path,
                list <NacosString> &headers,
                const NacosString &paramValues,
                const NacosString &encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    CURL *curlHandle = getCurlHandle();
    CURLcode curlres;

    NacosString Url = path;
    log_debug("[HTTPCli]-POST:Assembled URL with parms:%s\n", Url.c_str());

    /*Headers look like:
        foo
        bar
        bax
        lol
    We convert it into sth like with assembleHeaders():
        foo: bar
        bax: lol
    */
    list <NacosString> assembledHeaders;
    assembleHeaders(assembledHeaders, headers);

    log_debug("[HTTPCli]-POST:Post data:%s\n", paramValues.c_str());

    //clear-ups
    curl_easy_reset(curlHandle);
    /* specify URL to get */
    curl_easy_setopt(curlHandle, CURLOPT_URL, Url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, 1);

    curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, paramValues.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDSIZE, paramValues.size());
    //Setup common parameters
    HTTPBasicSettings(curlHandle);

    NacosString strbuf = "";
    /* we pass our 'strbuf' struct to the callback function */
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *) &strbuf);

    /* Get response headers from the response */
    std::map <NacosString, NacosString> respheaders;
    curl_easy_setopt(curlHandle, CURLOPT_HEADERDATA, (void *) &respheaders);

    //TODO:Time out in a more precise way
    curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT, readTimeoutMs / 1000);

    /*Add the request headers to the request*/
    struct curl_slist *headerlist = NULL;

    for (list<NacosString>::iterator it = assembledHeaders.begin(); it != assembledHeaders.end(); it++) {
        headerlist = curl_slist_append(headerlist, it->c_str());
        log_debug("[HTTPCli]-POST:RequestHeaders:%s\n", it->c_str());
    }

    if (headerlist != NULL) {
        curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headerlist);
    }


    /* get it! */
    curlres = curl_easy_perform(curlHandle);

    /*Since the headerlist is not needed anymore, free it to prevent mem leak*/
    if (headerlist != NULL) {
        curl_slist_free_all(headerlist);
        headerlist = NULL;
    }

    if (curlres != CURLE_OK) {
        log_error("[HTTPCli]-POST:curl_easy_perform() failed: %d - %s\n",
                  curlres, curl_easy_strerror(curlres));
        throw NetworkException(curlres, curl_easy_strerror(curlres));
    }

    long response_code;
    curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &response_code);
    HttpResult httpresp = HttpResult(response_code, strbuf, respheaders);
    httpresp.curlcode = curlres;

    log_debug("[HTTPCli]-POST:%lu bytes retrieved\n", (unsigned long) strbuf.length());
    log_debug("[HTTPCli]-POST:content:%s\n", strbuf.c_str());
    log_debug("[HTTPCli]-POST:resp-code:%d\n", response_code);

    return httpresp;
}

/*httpPut, put data are passed in list form like this:
	foo
	bar
	bax
	lol
We convert it into sth like this:
	foo=bar&bax=lol
*/
HttpResult HTTPCli::httpPut
        (
                const NacosString &path,
                list <NacosString> &headers,
                list <NacosString> &paramValues,
                const NacosString &encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    NacosString parmVal;
    parmVal = encodingParams(paramValues);
    return httpPutInternal(path, headers, parmVal, encoding, readTimeoutMs);
}

//httpPut, post data are passed in map form
HttpResult HTTPCli::httpPut
        (
                const NacosString &path,
                list <NacosString> &headers,
                map <NacosString, NacosString> &paramValues,
                const NacosString &encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    NacosString parmVal;
    parmVal = encodingParams(paramValues);
    return httpPutInternal(path, headers, parmVal, encoding, readTimeoutMs);
}

//Implement of httpPut
HttpResult HTTPCli::httpPutInternal
        (
                const NacosString &path,
                list <NacosString> &headers,
                const NacosString &paramValues,
                const NacosString &encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    CURL *curlHandle = getCurlHandle();
    CURLcode curlres;

    NacosString Url = path;
    log_debug("[HTTPCli]-PUT:Assembled URL with parms:%s\n", Url.c_str());

    /*Headers look like:
        foo
        bar
        bax
        lol
    We convert it into sth like with assembleHeaders():
        foo: bar
        bax: lol
    */
    list <NacosString> assembledHeaders;
    assembleHeaders(assembledHeaders, headers);

    log_debug("[HTTPCli]-PUT:Put data:%s\n", paramValues.c_str());

    //clear-ups
    curl_easy_reset(curlHandle);
    /* specify URL to get */
    curl_easy_setopt(curlHandle, CURLOPT_URL, Url.c_str());

    curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, paramValues.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDSIZE, paramValues.size());
    curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, 1);
    //Setup common parameters
    HTTPBasicSettings(curlHandle);

    NacosString strbuf = "";
    /* we pass our 'strbuf' struct to the callback function */
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *) &strbuf);

    /* Get response headers from the response */
    std::map <NacosString, NacosString> respheaders;
    curl_easy_setopt(curlHandle, CURLOPT_HEADERDATA, (void *) &respheaders);

    //TODO:Time out in a more precise way
    curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT, readTimeoutMs / 1000);

    /*Add the request headers to the request*/
    struct curl_slist *headerlist = NULL;

    for (list<NacosString>::iterator it = assembledHeaders.begin(); it != assembledHeaders.end(); it++) {
        headerlist = curl_slist_append(headerlist, it->c_str());
        log_debug("[HTTPCli]-PUT:RequestHeaders:%s\n", it->c_str());
    }

    if (headerlist != NULL) {
        curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headerlist);
    }


    /* get it! */
    curlres = curl_easy_perform(curlHandle);

    /*Since the headerlist is not needed anymore, free it to prevent mem leak*/
    if (headerlist != NULL) {
        curl_slist_free_all(headerlist);
        headerlist = NULL;
    }

    if (curlres != CURLE_OK) {
        log_error("[HTTPCli]-PUT:curl_easy_perform() failed: %d - %s\n",
                  curlres, curl_easy_strerror(curlres));
        throw NetworkException(curlres, curl_easy_strerror(curlres));
    }

    long response_code;
    curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &response_code);
    HttpResult httpresp = HttpResult(response_code, strbuf, respheaders);
    httpresp.curlcode = curlres;

    log_debug("[HTTPCli]-PUT:%lu bytes retrieved\n", (unsigned long) strbuf.length());
    log_debug("[HTTPCli]-PUT:content:%s\n", strbuf.c_str());
    log_debug("[HTTPCli]-PUT:resp-code:%d\n", response_code);

    return httpresp;
}

HttpResult HTTPCli::httpDelete
        (
                const NacosString &path,
                list <NacosString> &headers,
                list <NacosString> &paramValues,
                const NacosString &encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    NacosString parmVal;
    parmVal = encodingParams(paramValues);
    return httpDeleteInternal(path, headers, parmVal, encoding, readTimeoutMs);
}

HttpResult HTTPCli::httpDelete
        (
                const NacosString &path,
                list <NacosString> &headers,
                map <NacosString, NacosString> &paramValues,
                const NacosString &encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    NacosString parmVal;
    parmVal = encodingParams(paramValues);
    return httpDeleteInternal(path, headers, parmVal, encoding, readTimeoutMs);
}

HttpResult HTTPCli::httpDeleteInternal
        (
                const NacosString &path,
                list <NacosString> &headers,
                const NacosString &paramValues,
                const NacosString &encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    CURL *curlHandle = getCurlHandle();
    CURLcode curlres;

    NacosString Url = path;

    Url += "?" + paramValues;
    log_debug("[HTTPCli]-DELETE:Assembled URL with parms:%s\n", Url.c_str());

    /*Headers look like:
        foo
        bar
        bax
        lol
    We convert it into sth like with assembleHeaders():
        foo: bar
        bax: lol
    */
    list <NacosString> assembledHeaders;
    assembleHeaders(assembledHeaders, headers);

    //clear-ups
    curl_easy_reset(curlHandle);
    curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, 1);
    /* specify URL to get */
    curl_easy_setopt(curlHandle, CURLOPT_URL, Url.c_str());

    //Setup common parameters
    HTTPBasicSettings(curlHandle);
    /* Set to DELETE, since this is a delete request*/
    curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "DELETE");

    NacosString strbuf = "";
    /* we pass our 'strbuf' struct to the callback function */
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *) &strbuf);

    /* Get response headers from the response */
    std::map <NacosString, NacosString> respheaders;
    curl_easy_setopt(curlHandle, CURLOPT_HEADERDATA, (void *) &respheaders);

    //TODO:Time out in a more precise way
    curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT, readTimeoutMs / 1000);

    /*Add the request headers to the request*/
    struct curl_slist *headerlist = NULL;

    for (list<NacosString>::iterator it = assembledHeaders.begin(); it != assembledHeaders.end(); it++) {
        headerlist = curl_slist_append(headerlist, it->c_str());
    }

    if (headerlist != NULL) {
        curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headerlist);
    }


    /* get it! */
    curlres = curl_easy_perform(curlHandle);

    /*Since the headerlist is not needed anymore, free it to prevent mem leak*/
    if (headerlist != NULL) {
        curl_slist_free_all(headerlist);
        headerlist = NULL;
    }

    if (curlres != CURLE_OK) {
        log_error("[HTTPCli]-DELETE:curl_easy_perform() failed: %d - %s\n",
                  curlres, curl_easy_strerror(curlres));
        throw NetworkException(curlres, curl_easy_strerror(curlres));
    }

    long response_code;
    curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &response_code);
    HttpResult httpresp = HttpResult(response_code, strbuf, respheaders);
    httpresp.curlcode = curlres;

    log_debug("[HTTPCli]-DELETE:%lu bytes retrieved\n", (unsigned long) strbuf.length());
    log_debug("[HTTPCli]-DELETE:content:%s\n", strbuf.c_str());
    log_debug("[HTTPCli]-DELETE:resp-code:%d\n", response_code);

    return httpresp;
}

HTTPCli::~HTTPCli() {
    CURL *curlHandle = pthread_getspecific(pthreadKey);
    if (curlHandle != NULL) {
        curl_easy_cleanup(curlHandle);
    }
    pthread_key_delete(pthreadKey);
}

void HTTPCli::HTTP_GLOBAL_INIT() {
    curl_global_init(CURL_GLOBAL_ALL);
}

void HTTPCli::HTTP_GLOBAL_DEINIT() {
    curl_global_cleanup();
}
}//namespace nacos
