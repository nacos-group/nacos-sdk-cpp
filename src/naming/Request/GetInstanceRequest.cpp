#include <GetInstanceRequest.h>

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring GetInstanceRequest::getIp() const
	{
		return Ip;
	}

	void GetInstanceRequest::setIp(const std::wstring &value)
	{
		Ip = value;
	}

	int GetInstanceRequest::getPort() const
	{
		return Port;
	}

	void GetInstanceRequest::setPort(int value)
	{
		Port = value;
	}

	std::wstring GetInstanceRequest::getServiceName() const
	{
		return ServiceName;
	}

	void GetInstanceRequest::setServiceName(const std::wstring &value)
	{
		ServiceName = value;
	}

	std::wstring GetInstanceRequest::getNamespaceId() const
	{
		return NamespaceId;
	}

	void GetInstanceRequest::setNamespaceId(const std::wstring &value)
	{
		NamespaceId = value;
	}

	std::optional<bool> GetInstanceRequest::getHealthyOnly() const
	{
		return HealthyOnly;
	}

	void GetInstanceRequest::setHealthyOnly(const std::optional<bool> &value)
	{
		HealthyOnly = value;
	}

	std::wstring GetInstanceRequest::getCluster() const
	{
		return Cluster;
	}

	void GetInstanceRequest::setCluster(const std::wstring &value)
	{
		Cluster = value;
	}

	std::wstring GetInstanceRequest::getGroupName() const
	{
		return GroupName;
	}

	void GetInstanceRequest::setGroupName(const std::wstring &value)
	{
		GroupName = value;
	}

	std::optional<bool> GetInstanceRequest::getEphemeral() const
	{
		return Ephemeral;
	}

	void GetInstanceRequest::setEphemeral(const std::optional<bool> &value)
	{
		Ephemeral = value;
	}

	void GetInstanceRequest::CheckParam()
	{
		ParamUtil::CheckInstanceInfo(getIp(), getPort(), getServiceName());
	}

	std::unordered_map<std::wstring, std::wstring> GetInstanceRequest::ToDict()
	{
		auto dict = std::unordered_map<std::wstring, std::wstring>
		{
			{"serviceName", getServiceName()},
			{"ip", getIp()},
			{"port", std::to_wstring(getPort())}
		};

		if (!StringHelper::isEmptyOrWhiteSpace(getNamespaceId()))
		{
			dict.emplace("namespaceId", getNamespaceId());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getCluster()))
		{
			dict.emplace("cluster", getCluster());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getGroupName()))
		{
			dict.emplace("groupName", getGroupName());
		}

		if (getHealthyOnly())
		{

			dict.emplace("healthyOnly", getHealthyOnly().value().ToString());
		}

		if (getEphemeral())
		{

			dict.emplace("ephemeral", getEphemeral().value().ToString());
		}

		return dict;
	}
}
