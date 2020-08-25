#include <RemoveServiceRequest.h>

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring RemoveServiceRequest::getServiceName() const
	{
		return ServiceName;
	}

	void RemoveServiceRequest::setServiceName(const std::wstring &value)
	{
		ServiceName = value;
	}

	std::wstring RemoveServiceRequest::getNamespaceId() const
	{
		return NamespaceId;
	}

	void RemoveServiceRequest::setNamespaceId(const std::wstring &value)
	{
		NamespaceId = value;
	}

	std::wstring RemoveServiceRequest::getGroupName() const
	{
		return GroupName;
	}

	void RemoveServiceRequest::setGroupName(const std::wstring &value)
	{
		GroupName = value;
	}

	void RemoveServiceRequest::CheckParam()
	{
		ParamUtil::CheckServiceName(getServiceName());
	}

	std::unordered_map<std::wstring, std::wstring> RemoveServiceRequest::ToDict()
	{
		auto dict = std::unordered_map<std::wstring, std::wstring>
		{
			{"serviceName", getServiceName()}
		};

		if (!StringHelper::isEmptyOrWhiteSpace(getNamespaceId()))
		{
			dict.emplace("namespaceId", getNamespaceId());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getGroupName()))
		{
			dict.emplace("groupName", getGroupName());
		}

		return dict;
	}
}
