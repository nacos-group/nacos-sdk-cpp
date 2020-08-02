#include <RemoveInstanceRequest.h>

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring RemoveInstanceRequest::getIp() const
	{
		return Ip;
	}

	void RemoveInstanceRequest::setIp(const std::wstring &value)
	{
		Ip = value;
	}

	int RemoveInstanceRequest::getPort() const
	{
		return Port;
	}

	void RemoveInstanceRequest::setPort(int value)
	{
		Port = value;
	}

	std::wstring RemoveInstanceRequest::getServiceName() const
	{
		return ServiceName;
	}

	void RemoveInstanceRequest::setServiceName(const std::wstring &value)
	{
		ServiceName = value;
	}

	std::wstring RemoveInstanceRequest::getNamespaceId() const
	{
		return NamespaceId;
	}

	void RemoveInstanceRequest::setNamespaceId(const std::wstring &value)
	{
		NamespaceId = value;
	}

	std::wstring RemoveInstanceRequest::getClusterName() const
	{
		return ClusterName;
	}

	void RemoveInstanceRequest::setClusterName(const std::wstring &value)
	{
		ClusterName = value;
	}

	std::wstring RemoveInstanceRequest::getGroupName() const
	{
		return GroupName;
	}

	void RemoveInstanceRequest::setGroupName(const std::wstring &value)
	{
		GroupName = value;
	}

	std::optional<bool> RemoveInstanceRequest::getEphemeral() const
	{
		return Ephemeral;
	}

	void RemoveInstanceRequest::setEphemeral(const std::optional<bool> &value)
	{
		Ephemeral = value;
	}

	void RemoveInstanceRequest::CheckParam()
	{
		ParamUtil::CheckInstanceInfo(getIp(), getPort(), getServiceName());
	}

	std::unordered_map<std::wstring, std::wstring> RemoveInstanceRequest::ToDict()
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

		if (!StringHelper::isEmptyOrWhiteSpace(getClusterName()))
		{
			dict.emplace("clusterName", getClusterName());
		}

		if (getEphemeral())
		{

			dict.emplace("ephemeral", getEphemeral().value().ToString());
		}

		return dict;
	}
}
