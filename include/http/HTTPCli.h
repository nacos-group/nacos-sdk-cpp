#ifndef __HTTP_CLI_H_
#define __HTTP_CLI_H_

#include <list>
#include <map>
#include <pthread.h>
#include <curl/curl.h>
#include "NacosString.h"
#include "NacosExceptions.h"

class HttpResult {
public:
	long code;
	NacosString content;
	std::map< NacosString, NacosString > headers;
	CURLcode curlcode;
	HttpResult(long _code, const NacosString &_content, std::map< NacosString, NacosString > &_headers)
	: code(_code), content(_content)
	{
		headers.insert(_headers.begin(), _headers.end());
	}

	HttpResult(long _code, const NacosString &_content): code(_code), content(_content) {}
	HttpResult() { code = -1; content = ""; headers.clear(); }

	HttpResult operator = (HttpResult asignee)
	{
		if (this != &asignee)
		{
			headers.insert(asignee.headers.begin(), asignee.headers.end());
			code = asignee.code;
			content = asignee.content;
			curlcode = asignee.curlcode;
		}

		return *this;
	}
};

class HTTPCli
{
private:
	//CURL *curlHandle;
	pthread_key_t pthreadKey;

	CURL *getCurlHandle();
	static void destroyCurlHandle(void* arg);
	HttpResult httpGetInternal
	(
		const NacosString &path,
		std::list<NacosString> &headers,
		const NacosString &paramValues,
		const NacosString &encoding,
		long readTimeoutMs
	) throw (NetworkException);
	HttpResult httpPostInternal
	(
		const NacosString &path,
		std::list<NacosString> &headers,
		const NacosString &paramValues,
		const NacosString &encoding,
		long readTimeoutMs
	) throw (NetworkException);
	HttpResult httpPutInternal
	(
		const NacosString &path,
		std::list<NacosString> &headers,
		const NacosString &paramValues,
		const NacosString &encoding,
		long readTimeoutMs
	) throw (NetworkException);
	HttpResult httpDeleteInternal
	(
		const NacosString &path,
		std::list<NacosString> &headers,
		const NacosString &paramValues,
		const NacosString &encoding,
		long readTimeoutMs
	) throw (NetworkException);
public:
	static NacosString encodingParams(std::list<NacosString> &params);
	static NacosString encodingParams(std::map<NacosString, NacosString> &params);
	static void assembleHeaders(std::list<NacosString> &assembledHeaders, std::list<NacosString> &headers);
	static void HTTPBasicSettings(CURL *curlHandle);
	static void HTTP_GLOBAL_INIT();
	static void HTTP_GLOBAL_DEINIT();
	HTTPCli();
	~HTTPCli();
	HttpResult httpGet(
		const NacosString &path,
		std::list<NacosString> &headers,
		std::list<NacosString> &paramValues,
		const NacosString &encoding,
		long readTimeoutMs
	) throw (NetworkException);
	HttpResult httpGet(
		const NacosString &path,
		std::list<NacosString> &headers,
		std::map<NacosString, NacosString> &paramValues,
		const NacosString &encoding,
		long readTimeoutMs
	) throw (NetworkException);
	HttpResult httpDelete(
		const NacosString &path,
		std::list<NacosString> &headers,
		std::list<NacosString> &paramValues,
		const NacosString &encoding,
		long readTimeoutMs
	) throw (NetworkException);
	HttpResult httpDelete(
		const NacosString &path,
		std::list<NacosString> &headers,
		std::map<NacosString, NacosString> &paramValues,
		const NacosString &encoding,
		long readTimeoutMs
	) throw (NetworkException);
	HttpResult httpPost(
		const NacosString &path,
		std::list<NacosString> &headers,
		std::list<NacosString> &paramValues,
		const NacosString &encoding,
		long readTimeoutMs
	) throw (NetworkException);
	HttpResult httpPost(
		const NacosString &path,
		std::list<NacosString> &headers,
		std::map<NacosString, NacosString> &paramValues,
		const NacosString &encoding,
		long readTimeoutMs
	) throw (NetworkException);
	HttpResult httpPut(
		const NacosString &path,
		std::list<NacosString> &headers,
		std::list<NacosString> &paramValues,
		const NacosString &encoding,
		long readTimeoutMs
	) throw (NetworkException);
	HttpResult httpPut(
		const NacosString &path,
		std::list<NacosString> &headers,
		std::map<NacosString, NacosString> &paramValues,
		const NacosString &encoding,
		long readTimeoutMs
	) throw (NetworkException);
	static NacosString getPrefix() { return  "http://"; };//TODO:changeable according to env variable
	static const int GET = 0;
	static const int PUT = 1;
	static const int POST = 3;
	static const int DELETE = 4;
};

#endif
