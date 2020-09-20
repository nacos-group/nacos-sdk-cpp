#include "http/HTTPCli.h"
#include "Debug.h"
#include "DebugAssertion.h"
#include <iostream>

using namespace std;
#define DEFAULT_ENCODING "UTF-8"

bool testNormalHttpRequest() {
    cout << "in function testNormalHttpRequest" << endl;
    NacosString path = "http://127.0.0.1:8848/nacos/v1/ns/operator/servers";
    NacosString ENCODING = DEFAULT_ENCODING;
    std::list <NacosString> headers;
    std::list <NacosString> paramValues;
    HTTPCli httpcli;
    HttpResult callres;
    try {
        callres = httpcli.httpGet(path,
                                  headers,
                                  paramValues,
                                  ENCODING,
                                  1000);
    }
    catch (NetworkException e) {
        cout <<
             "Request failed with curl code:" << e.errorcode() << endl <<
             "Reason:" << e.what() << endl;
        return false;
    }

    cout << "Http Request returned with code:" << callres.code << endl;
    cout << "Headers:" << endl;

    for (std::map<NacosString, NacosString>::iterator it = callres.headers.begin();
         it != callres.headers.end(); ++it) {
        cout << it->first << ":" << it->second << endl;
    }

    cout << "Content:" << callres.content << endl;
    return true;
}

bool testNoServerRequest() {
    cout << "in function testNoServerRequest" << endl;
    NacosString path = "http://127.0.0.1:9999/nacos/v1/ns/operator/servers";
    NacosString ENCODING = DEFAULT_ENCODING;
    std::list <NacosString> headers;
    std::list <NacosString> paramValues;
    HTTPCli httpcli;
    HttpResult callres;
    try {
        callres = httpcli.httpGet(path,
                                  headers,
                                  paramValues,
                                  ENCODING,
                                  1000);
    }
    catch (NetworkException e) {
        //should throw a exception
        cout <<
             "Request failed with curl code:" << e.errorcode() << endl <<
             "Reason:" << e.what() << endl;
        return true;
    }

    cout << "Http Request returned with code:" << callres.code << endl;
    cout << "Headers:" << endl;

    for (std::map<NacosString, NacosString>::iterator it = callres.headers.begin();
         it != callres.headers.end(); ++it) {
        cout << it->first << ":" << it->second << endl;
    }

    cout << "Content:" << callres.content << endl;
    return false;
}