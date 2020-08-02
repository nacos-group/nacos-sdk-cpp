#include <ModifyInstanceHealthStatusRequest.h>

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring ModifyInstanceHealthStatusRequest::getIp() const
	{
		return Ip;
	}

	void ModifyInstanceHealthStatusRequest::setIp(const std::wstring &value)
	{
		Ip = value;
	}

	int ModifyInstanceHealthStatusRequest::getPort() const
	{
		return Port;
	}

	void ModifyInstanceHealthStatusRequest::setPort(int value)
	{
		Port = value;
	}

	std::wstring ModifyInstanceHealthStatusRequest::getServiceName() const
	{
		return ServiceName;
	}

	void ModifyInstanceHealthStatusRequest::setServiceName(const std::wstring &value)
	{
		ServiceName = value;
	}

	std::wstring ModifyInstanceHealthStatusRequest::getNamespaceId() const
	{
		return NamespaceId;
	}

	void ModifyInstanceHealthStatusRequest::setNamespaceId(const std::wstring &value)
	{
		NamespaceId = value;
	}

	bool ModifyInstanceHealthStatusRequest::getHealthy() const
	{
		return Healthy;
	}

	void ModifyInstanceHealthStatusRequest::setHealthy(bool value)
	{
		Healthy = value;
	}

	std::wstring ModifyInstanceHealthStatusRequest::getClusterName() const
	{
		return ClusterName;
	}

	void ModifyInstanceHealthStatusRequest::setClusterName(const std::wstring &value)
	{
		ClusterName = value;
	}

	std::wstring ModifyInstanceHealthStatusRequest::getGroupName() const
	{
		return GroupName;
	}

	void ModifyInstanceHealthStatusRequest::setGroupName(const std::wstring &value)
	{
		GroupName = value;
	}

	void ModifyInstanceHealthStatusRequest::CheckParam()
	{
		ParamUtil::CheckInstanceInfo(getIp(), getPort(), getServiceName());
	}

	std::unordered_map<std::wstring, std::wstring> ModifyInstanceHealthStatusRequest::ToDict()
	{
		auto dict = std::unordered_map<std::wstring, std::wstring>
		{
			{"serviceName", getServiceName()},
			{"ip", getIp()},
			{"port", std::to_wstring(getPort())},
			{"healthy", StringHelper::toString(getHealthy())}
		};

		if (!StringHelper::isEmptyOrWhiteSpace(getNamespaceId()))
		{
			dict.emplace("namespaceId", getNamespaceId());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getGroupName()))
		{
			dict.emplace("groupName", getGroupName());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getClusterName()))
		{
			dict.emplace("clusterName", getClusterName());
		}

		return dict;
	}
}
