#include <string.h>
#include "HTTPCli.h"
#include "utils/url.h"
#include "Debug.h"

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

    log_debug("receivedHeaders: %s", (char *) contents);

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
    } else//TODO:Temporary solution for 400 response when performing multiple post request with one curl handle
    {
        curl_easy_cleanup(curlHandle);
        curlHandle = NULL;
        pthread_setspecific(pthreadKey, reinterpret_cast<void *>(curlHandle));
        return getCurlHandle();
    }//TODO:END:Temporary solution for 400 response when performing multiple post request with one curl handle

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
    curl_easy_setopt(curlHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, receiveResponseCallback);
    curl_easy_setopt(curlHandle, CURLOPT_HEADERFUNCTION, receiveHeaderCallback);
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
        ) throw(NetworkException) {
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
        ) throw(NetworkException) {
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
        ) throw(NetworkException) {
    CURL *curlHandle = getCurlHandle();
    CURLcode curlres;

    NacosString Url = path;

    if (paramValues.compare("") != 0) {
        Url += "?" + paramValues;
    }
    log_debug("HTTPGET-Assembled URL with parms:%s\n", Url.c_str());

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

    /* specify URL to get */
    curl_easy_setopt(curlHandle, CURLOPT_URL, Url.c_str());

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
        log_error("curl_easy_perform() failed: %d - %s\n",
                  curlres, curl_easy_strerror(curlres));
        throw NetworkException(curlres, curl_easy_strerror(curlres));
    }

    long response_code;
    curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &response_code);
    HttpResult httpresp = HttpResult(response_code, strbuf, respheaders);
    httpresp.curlcode = curlres;
    log_debug("HTTPGET-%lu bytes retrieved\n", (unsigned long) strbuf.length());
    log_debug("HTTPGET-content:%s\n", strbuf.c_str());
    log_debug("HTTPGET-resp-code:%d\n", response_code);

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
        ) throw(NetworkException) {
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
        ) throw(NetworkException) {
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
        ) throw(NetworkException) {
    CURL *curlHandle = getCurlHandle();
    CURLcode curlres;

    NacosString Url = path;
    log_debug("HTTPPOST-Assembled URL with parms:%s\n", Url.c_str());

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

    log_debug("Post data:%s\n", paramValues.c_str());

    /* specify URL to get */
    curl_easy_setopt(curlHandle, CURLOPT_URL, Url.c_str());

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
        log_debug("HTTPPOST-RequestHeaders:%s\n", it->c_str());
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
        log_error("curl_easy_perform() failed: %d - %s\n",
                  curlres, curl_easy_strerror(curlres));
        throw NetworkException(curlres, curl_easy_strerror(curlres));
    }

    long response_code;
    curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &response_code);
    HttpResult httpresp = HttpResult(response_code, strbuf, respheaders);
    httpresp.curlcode = curlres;

    log_debug("HTTPPOST-%lu bytes retrieved\n", (unsigned long) strbuf.length());
    log_debug("HTTPPOST-content:%s\n", strbuf.c_str());
    log_debug("HTTPPOST-resp-code:%d\n", response_code);

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
        ) throw(NetworkException) {
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
        ) throw(NetworkException) {
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
        ) throw(NetworkException) {
    CURL *curlHandle = getCurlHandle();
    CURLcode curlres;

    NacosString Url = path;
    log_debug("HTTPPUT-Assembled URL with parms:%s\n", Url.c_str());

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

    log_debug("Put data:%s\n", paramValues.c_str());

    /* specify URL to get */
    curl_easy_setopt(curlHandle, CURLOPT_URL, Url.c_str());

    curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "PUT");
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
        log_debug("HTTPPUT-RequestHeaders:%s\n", it->c_str());
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
        log_error("curl_easy_perform() failed: %d - %s\n",
                  curlres, curl_easy_strerror(curlres));
        throw NetworkException(curlres, curl_easy_strerror(curlres));
    }

    long response_code;
    curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &response_code);
    HttpResult httpresp = HttpResult(response_code, strbuf, respheaders);
    httpresp.curlcode = curlres;

    log_debug("HTTPPUT-%lu bytes retrieved\n", (unsigned long) strbuf.length());
    log_debug("HTTPPUT-content:%s\n", strbuf.c_str());
    log_debug("HTTPPUT-resp-code:%d\n", response_code);

    return httpresp;
}

HttpResult HTTPCli::httpDelete
        (
                const NacosString &path,
                list <NacosString> &headers,
                list <NacosString> &paramValues,
                const NacosString &encoding,
                long readTimeoutMs
        ) throw(NetworkException) {
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
        ) throw(NetworkException) {
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
        ) throw(NetworkException) {
    CURL *curlHandle = getCurlHandle();
    CURLcode curlres;

    NacosString Url = path;

    Url += "?" + paramValues;
    log_debug("Assembled URL with parms:%s\n", Url.c_str());

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
        log_error("curl_easy_perform() failed: %d - %s\n",
                  curlres, curl_easy_strerror(curlres));
        throw NetworkException(curlres, curl_easy_strerror(curlres));
    }

    long response_code;
    curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &response_code);
    HttpResult httpresp = HttpResult(response_code, strbuf, respheaders);
    httpresp.curlcode = curlres;

    log_debug("HTTPDELETE-%lu bytes retrieved\n", (unsigned long) strbuf.length());
    log_debug("HTTPDELETE-content:%s\n", strbuf.c_str());
    log_debug("HTTPDELETE-resp-code:%d\n", response_code);

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
