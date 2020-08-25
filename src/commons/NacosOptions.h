#pragma once

#include <string>
#include <vector>

namespace Nacos
{

	class NacosOptions
	{
	private:
		std::vector<std::wstring> ServerAddresses;
		std::wstring EndPoint;
		std::wstring ContextPath = L"nacos";
		std::wstring ClusterName = L"serverlist";
		int DefaultTimeOut = 15000;
		std::wstring Namespace = L"";
		std::wstring AccessKey;
		std::wstring SecretKey;
		std::wstring UserName;
		std::wstring Password;
		int ListenInterval = 1000;

		/// <summary>
		/// nacos server addresses.
		/// </summary>
		/// <example>
		/// http://10.1.12.123:8848,https://10.1.12.124:8848
		/// </example>
	public:
		std::vector<std::wstring> getServerAddresses() const;
		void setServerAddresses(const std::vector<std::wstring> &value);

		/// <summary>
		/// EndPoint
		/// </summary>
		std::wstring getEndPoint() const;
		void setEndPoint(const std::wstring &value);

		std::wstring getContextPath() const;
		void setContextPath(const std::wstring &value);

		std::wstring getClusterName() const;
		void setClusterName(const std::wstring &value);

		/// <summary>
		/// default timeout, unit is Milliseconds.
		/// </summary>
		int getDefaultTimeOut() const;
		void setDefaultTimeOut(int value);

		/// <summary>
		/// default namespace
		/// </summary>
		std::wstring getNamespace() const;
		void setNamespace(const std::wstring &value);

		/// <summary>
		/// accessKey
		/// </summary>
		std::wstring getAccessKey() const;
		void setAccessKey(const std::wstring &value);

		/// <summary>
		/// secretKey
		/// </summary>
		std::wstring getSecretKey() const;
		void setSecretKey(const std::wstring &value);

		std::wstring getUserName() const;
		void setUserName(const std::wstring &value);

		std::wstring getPassword() const;
		void setPassword(const std::wstring &value);

		/// <summary>
		/// listen interval, unit is millisecond.
		/// </summary>
		int getListenInterval() const;
		void setListenInterval(int value);
	};
}
