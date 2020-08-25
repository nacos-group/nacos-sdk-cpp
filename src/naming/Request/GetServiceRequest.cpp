#include "GetServiceRequest.h"

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring GetServiceRequest::getServiceName() const
	{
		return ServiceName;
	}

	void GetServiceRequest::setServiceName(const std::wstring &value)
	{
		ServiceName = value;
	}

	std::wstring GetServiceRequest::getNamespaceId() const
	{
		return NamespaceId;
	}

	void GetServiceRequest::setNamespaceId(const std::wstring &value)
	{
		NamespaceId = value;
	}

	std::wstring GetServiceRequest::getGroupName() const
	{
		return GroupName;
	}

	void GetServiceRequest::setGroupName(const std::wstring &value)
	{
		GroupName = value;
	}

	void GetServiceRequest::CheckParam()
	{
		ParamUtil::CheckServiceName(getServiceName());
	}

	std::unordered_map<std::wstring, std::wstring> GetServiceRequest::ToDict()
	{
		auto dict = std::unordered_map<std::wstring, std::wstring>
		{
			{L"serviceName", getServiceName()}
		};

		if (!StringHelper::isEmptyOrWhiteSpace(getNamespaceId()))
		{
			dict.emplace(L"namespaceId", getNamespaceId());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getGroupName()))
		{
			dict.emplace(L"groupName", getGroupName());
		}

		return dict;
	}
}
