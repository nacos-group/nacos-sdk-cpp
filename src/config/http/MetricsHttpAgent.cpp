#include <MetricsHttpAgent.h>

namespace com::alibaba::nacos::client::config::http
{


	MetricsHttpAgent::MetricsHttpAgent(HttpAgent *httpAgent) : httpAgent(httpAgent)
	{
	}

	void MetricsHttpAgent::start()
	{
		httpAgent->start();
	}

	HttpResult *MetricsHttpAgent::httpGet(const std::wstring &path, std::vector<std::wstring> &headers, std::vector<std::wstring> &paramValues, const std::wstring &encoding, long long readTimeoutMs)
	{
		Histogram::Timer *timer = MetricsMonitor::getConfigRequestMonitor("GET", path, "NA");
		HttpResult *result;
		try
		{
			result = httpAgent->httpGet(path, headers, paramValues, encoding, readTimeoutMs);
		}
		catch (const IOException &e)
		{
			throw e;
		}

		finally
		{
			timer->observeDuration();
			timer->close();
		}

		return result;
	}

	HttpResult *MetricsHttpAgent::httpPost(const std::wstring &path, std::vector<std::wstring> &headers, std::vector<std::wstring> &paramValues, const std::wstring &encoding, long long readTimeoutMs)
	{
		Histogram::Timer *timer = MetricsMonitor::getConfigRequestMonitor("POST", path, "NA");
		HttpResult *result;
		try
		{
			result = httpAgent->httpPost(path, headers, paramValues, encoding, readTimeoutMs);
		}
		catch (const IOException &e)
		{
			throw e;
		}

		finally
		{
			timer->observeDuration();
			timer->close();
		}

		return result;
	}

	HttpResult *MetricsHttpAgent::httpDelete(const std::wstring &path, std::vector<std::wstring> &headers, std::vector<std::wstring> &paramValues, const std::wstring &encoding, long long readTimeoutMs)
	{
		Histogram::Timer *timer = MetricsMonitor::getConfigRequestMonitor("DELETE", path, "NA");
		HttpResult *result;
		try
		{
			result = httpAgent->httpDelete(path, headers, paramValues, encoding, readTimeoutMs);
		}
		catch (const IOException &e)
		{

			throw e;
		}

		finally
		{
			timer->observeDuration();
			timer->close();
		}

		return result;
	}

	std::wstring MetricsHttpAgent::getName()
	{
		return httpAgent->getName();
	}

	std::wstring MetricsHttpAgent::getNamespace()
	{
		return httpAgent->getNamespace();
	}

	std::wstring MetricsHttpAgent::getTenant()
	{
		return httpAgent->getTenant();
	}

	std::wstring MetricsHttpAgent::getEncode()
	{
		return httpAgent->getEncode();
	}

	void MetricsHttpAgent::shutdown()
	{
		httpAgent->shutdown();
	}
}
