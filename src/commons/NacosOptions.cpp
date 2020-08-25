#include "NacosOptions.h"

namespace Nacos
{

	std::vector<std::wstring> NacosOptions::getServerAddresses() const
	{
		return ServerAddresses;
	}

	void NacosOptions::setServerAddresses(const std::vector<std::wstring> &value)
	{
		ServerAddresses = value;
	}

	std::wstring NacosOptions::getEndPoint() const
	{
		return EndPoint;
	}

	void NacosOptions::setEndPoint(const std::wstring &value)
	{
		EndPoint = value;
	}

	std::wstring NacosOptions::getContextPath() const
	{
		return ContextPath;
	}

	void NacosOptions::setContextPath(const std::wstring &value)
	{
		ContextPath = value;
	}

	std::wstring NacosOptions::getClusterName() const
	{
		return ClusterName;
	}

	void NacosOptions::setClusterName(const std::wstring &value)
	{
		ClusterName = value;
	}

	int NacosOptions::getDefaultTimeOut() const
	{
		return DefaultTimeOut;
	}

	void NacosOptions::setDefaultTimeOut(int value)
	{
		DefaultTimeOut = value;
	}

	std::wstring NacosOptions::getNamespace() const
	{
		return Namespace;
	}

	void NacosOptions::setNamespace(const std::wstring &value)
	{
		Namespace = value;
	}

	std::wstring NacosOptions::getAccessKey() const
	{
		return AccessKey;
	}

	void NacosOptions::setAccessKey(const std::wstring &value)
	{
		AccessKey = value;
	}

	std::wstring NacosOptions::getSecretKey() const
	{
		return SecretKey;
	}

	void NacosOptions::setSecretKey(const std::wstring &value)
	{
		SecretKey = value;
	}

	std::wstring NacosOptions::getUserName() const
	{
		return UserName;
	}

	void NacosOptions::setUserName(const std::wstring &value)
	{
		UserName = value;
	}

	std::wstring NacosOptions::getPassword() const
	{
		return Password;
	}

	void NacosOptions::setPassword(const std::wstring &value)
	{
		Password = value;
	}

	int NacosOptions::getListenInterval() const
	{
		return ListenInterval;
	}

	void NacosOptions::setListenInterval(int value)
	{
		ListenInterval = value;
	}
}
