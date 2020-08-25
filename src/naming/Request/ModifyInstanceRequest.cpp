#include "ModifyInstanceRequest.h"

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring ModifyInstanceRequest::getIp() const
	{
		return Ip;
	}

	void ModifyInstanceRequest::setIp(const std::wstring &value)
	{
		Ip = value;
	}

	int ModifyInstanceRequest::getPort() const
	{
		return Port;
	}

	void ModifyInstanceRequest::setPort(int value)
	{
		Port = value;
	}

	std::wstring ModifyInstanceRequest::getServiceName() const
	{
		return ServiceName;
	}

	void ModifyInstanceRequest::setServiceName(const std::wstring &value)
	{
		ServiceName = value;
	}

	std::wstring ModifyInstanceRequest::getNamespaceId() const
	{
		return NamespaceId;
	}

	void ModifyInstanceRequest::setNamespaceId(const std::wstring &value)
	{
		NamespaceId = value;
	}

	std::optional<double> ModifyInstanceRequest::getWeight() const
	{
		return Weight;
	}

	void ModifyInstanceRequest::setWeight(const std::optional<double> &value)
	{
		Weight = value;
	}

	std::wstring ModifyInstanceRequest::getMetadata() const
	{
		return Metadata;
	}

	void ModifyInstanceRequest::setMetadata(const std::wstring &value)
	{
		Metadata = value;
	}

	std::wstring ModifyInstanceRequest::getClusterName() const
	{
		return ClusterName;
	}

	void ModifyInstanceRequest::setClusterName(const std::wstring &value)
	{
		ClusterName = value;
	}

	std::wstring ModifyInstanceRequest::getGroupName() const
	{
		return GroupName;
	}

	void ModifyInstanceRequest::setGroupName(const std::wstring &value)
	{
		GroupName = value;
	}

	std::optional<bool> ModifyInstanceRequest::getEphemeral() const
	{
		return Ephemeral;
	}

	void ModifyInstanceRequest::setEphemeral(const std::optional<bool> &value)
	{
		Ephemeral = value;
	}

	std::optional<bool> ModifyInstanceRequest::getEnabled() const
	{
		return Enabled;
	}

	void ModifyInstanceRequest::setEnabled(const std::optional<bool> &value)
	{
		Enabled = value;
	}

	void ModifyInstanceRequest::CheckParam()
	{
		ParamUtil::CheckInstanceInfo(getIp(), getPort(), getServiceName());
	}

	std::unordered_map<std::wstring, std::wstring> ModifyInstanceRequest::ToDict()
	{
		auto dict = std::unordered_map<std::wstring, std::wstring>
		{
			{L"serviceName", getServiceName()},
			{L"ip", getIp()},
			{L"port", std::to_wstring(getPort())}
		};

		if (!StringHelper::isEmptyOrWhiteSpace(getNamespaceId()))
		{
			dict.emplace(L"namespaceId", getNamespaceId());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getGroupName()))
		{
			dict.emplace(L"groupName", getGroupName());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getClusterName()))
		{
			dict.emplace(L"clusterName", getClusterName());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getMetadata()))
		{
			dict.emplace(L"metadata", getMetadata());
		}

		if (getEphemeral())
		{
//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to 'ToString':
			dict.emplace(L"ephemeral", getEphemeral().ToString());
		}

		if (getEnabled())
		{
//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to 'ToString':
			dict.emplace(L"enabled", getEnabled().value().ToString());
		}

		if (getWeight())
		{
//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to 'ToString':
			dict.emplace(L"weight", getWeight().value().ToString());
		}

		return dict;
	}
}
