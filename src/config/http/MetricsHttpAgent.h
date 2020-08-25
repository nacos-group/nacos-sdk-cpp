#pragma once

#include "HttpAgent.h"
#include <string>
#include <vector>
#include "exceptionhelper.h"

namespace com::alibaba::nacos::client::config::http { class HttpAgent; }


namespace com::alibaba::nacos::client::config::http
{

	using NacosException = com::alibaba::nacos::api::exception::NacosException;
	using HttpResult = com::alibaba::nacos::client::config::impl::HttpSimpleClient::HttpResult;

	using IOException = java::io::IOException;
	using List = java::util::List;

	/**
	 * MetricsHttpAgent.
	 *
	 * @author Nacos
	 */
	class MetricsHttpAgent : public HttpAgent
	{

	private:
		HttpAgent *const httpAgent;

	public:
		virtual ~MetricsHttpAgent()
		{
			delete httpAgent;
		}

		MetricsHttpAgent(HttpAgent *httpAgent);

		void start() override;

		HttpResult *httpGet(const std::wstring &path, std::vector<std::wstring> &headers, std::vector<std::wstring> &paramValues, const std::wstring &encoding, long long readTimeoutMs) override;

		HttpResult *httpPost(const std::wstring &path, std::vector<std::wstring> &headers, std::vector<std::wstring> &paramValues, const std::wstring &encoding, long long readTimeoutMs) override;

		HttpResult *httpDelete(const std::wstring &path, std::vector<std::wstring> &headers, std::vector<std::wstring> &paramValues, const std::wstring &encoding, long long readTimeoutMs) override;

		std::wstring getName() override;

		std::wstring getNamespace() override;

		std::wstring getTenant() override;

		std::wstring getEncode() override;

		void shutdown() override;
	};


}
