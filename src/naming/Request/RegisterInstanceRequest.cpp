#include <RegisterInstanceRequest.h>

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring RegisterInstanceRequest::getIp() const
	{
		return Ip;
	}

	void RegisterInstanceRequest::setIp(const std::wstring &value)
	{
		Ip = value;
	}

	int RegisterInstanceRequest::getPort() const
	{
		return Port;
	}

	void RegisterInstanceRequest::setPort(int value)
	{
		Port = value;
	}

	std::wstring RegisterInstanceRequest::getServiceName() const
	{
		return ServiceName;
	}

	void RegisterInstanceRequest::setServiceName(const std::wstring &value)
	{
		ServiceName = value;
	}

	std::wstring RegisterInstanceRequest::getNamespaceId() const
	{
		return NamespaceId;
	}

	void RegisterInstanceRequest::setNamespaceId(const std::wstring &value)
	{
		NamespaceId = value;
	}

	std::optional<double> RegisterInstanceRequest::getWeight() const
	{
		return Weight;
	}

	void RegisterInstanceRequest::setWeight(const std::optional<double> &value)
	{
		Weight = value;
	}

	std::optional<bool> RegisterInstanceRequest::getEnable() const
	{
		return Enable;
	}

	void RegisterInstanceRequest::setEnable(const std::optional<bool> &value)
	{
		Enable = value;
	}

	std::optional<bool> RegisterInstanceRequest::getHealthy() const
	{
		return Healthy;
	}

	void RegisterInstanceRequest::setHealthy(const std::optional<bool> &value)
	{
		Healthy = value;
	}

	std::wstring RegisterInstanceRequest::getMetadata() const
	{
		return Metadata;
	}

	void RegisterInstanceRequest::setMetadata(const std::wstring &value)
	{
		Metadata = value;
	}

	std::wstring RegisterInstanceRequest::getClusterName() const
	{
		return ClusterName;
	}

	void RegisterInstanceRequest::setClusterName(const std::wstring &value)
	{
		ClusterName = value;
	}

	std::wstring RegisterInstanceRequest::getGroupName() const
	{
		return GroupName;
	}

	void RegisterInstanceRequest::setGroupName(const std::wstring &value)
	{
		GroupName = value;
	}

	std::optional<bool> RegisterInstanceRequest::getEphemeral() const
	{
		return Ephemeral;
	}

	void RegisterInstanceRequest::setEphemeral(const std::optional<bool> &value)
	{
		Ephemeral = value;
	}

	void RegisterInstanceRequest::CheckParam()
	{
		ParamUtil::CheckInstanceInfo(getIp(), getPort(), getServiceName());
	}

	std::unordered_map<std::wstring, std::wstring> RegisterInstanceRequest::ToDict()
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

		if (!StringHelper::isEmptyOrWhiteSpace(getGroupName()))
		{
			dict.emplace("groupName", getGroupName());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getMetadata()))
		{
			dict.emplace("metadata", getMetadata());
		}

		if (getWeight())
		{

			dict.emplace("weight", getWeight().value().ToString());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getClusterName()))
		{
			dict.emplace("clusterName", getClusterName());
		}

		if (getEphemeral())
		{

			dict.emplace("ephemeral", getEphemeral().value().ToString());
		}

		if (getHealthy())
		{

			dict.emplace("healthy", getHealthy().value().ToString());
		}

		return dict;
	}
}
