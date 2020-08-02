#include <ListInstancesRequest.h>

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring ListInstancesRequest::getServiceName() const
	{
		return ServiceName;
	}

	void ListInstancesRequest::setServiceName(const std::wstring &value)
	{
		ServiceName = value;
	}

	std::wstring ListInstancesRequest::getNamespaceId() const
	{
		return NamespaceId;
	}

	void ListInstancesRequest::setNamespaceId(const std::wstring &value)
	{
		NamespaceId = value;
	}

	std::wstring ListInstancesRequest::getClusters() const
	{
		return Clusters;
	}

	void ListInstancesRequest::setClusters(const std::wstring &value)
	{
		Clusters = value;
	}

	std::wstring ListInstancesRequest::getGroupName() const
	{
		return GroupName;
	}

	void ListInstancesRequest::setGroupName(const std::wstring &value)
	{
		GroupName = value;
	}

	std::optional<bool> ListInstancesRequest::getHealthyOnly() const
	{
		return HealthyOnly;
	}

	void ListInstancesRequest::setHealthyOnly(const std::optional<bool> &value)
	{
		HealthyOnly = value;
	}

	void ListInstancesRequest::CheckParam()
	{
		ParamUtil::CheckServiceName(getServiceName());
	}

	std::unordered_map<std::wstring, std::wstring> ListInstancesRequest::ToDict()
	{
		auto dict = std::unordered_map<std::wstring, std::wstring>
		{
			{"serviceName", getServiceName()}
		};

		if (!StringHelper::isEmptyOrWhiteSpace(getNamespaceId()))
		{
			dict.emplace("namespaceId", getNamespaceId());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getClusters()))
		{
			dict.emplace("clusters", getClusters());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getGroupName()))
		{
			dict.emplace("groupName", getGroupName());
		}

		if (getHealthyOnly())
		{

			dict.emplace("healthyOnly", getHealthyOnly().value().ToString());
		}

		return dict;
	}
}
