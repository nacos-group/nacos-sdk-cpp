#include "ModifyServiceRequest.h"

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring ModifyServiceRequest::getServiceName() const
	{
		return ServiceName;
	}

	void ModifyServiceRequest::setServiceName(const std::wstring &value)
	{
		ServiceName = value;
	}

	std::wstring ModifyServiceRequest::getNamespaceId() const
	{
		return NamespaceId;
	}

	void ModifyServiceRequest::setNamespaceId(const std::wstring &value)
	{
		NamespaceId = value;
	}

	std::optional<float> ModifyServiceRequest::getProtectThreshold() const
	{
		return ProtectThreshold;
	}

	void ModifyServiceRequest::setProtectThreshold(const std::optional<float> &value)
	{
		ProtectThreshold = value;
	}

	std::wstring ModifyServiceRequest::getSelector() const
	{
		return Selector;
	}

	void ModifyServiceRequest::setSelector(const std::wstring &value)
	{
		Selector = value;
	}

	std::wstring ModifyServiceRequest::getMetadata() const
	{
		return Metadata;
	}

	void ModifyServiceRequest::setMetadata(const std::wstring &value)
	{
		Metadata = value;
	}

	std::wstring ModifyServiceRequest::getGroupName() const
	{
		return GroupName;
	}

	void ModifyServiceRequest::setGroupName(const std::wstring &value)
	{
		GroupName = value;
	}

	void ModifyServiceRequest::CheckParam()
	{
		ParamUtil::CheckServiceName(getServiceName());
	}

	std::unordered_map<std::wstring, std::wstring> ModifyServiceRequest::ToDict()
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

		if (!StringHelper::isEmptyOrWhiteSpace(getMetadata()))
		{
			dict.emplace(L"metadata", getMetadata());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getSelector()))
		{
			dict.emplace(L"selector", getSelector());
		}

		if (getProtectThreshold())
		{
//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to 'ToString':
			dict.emplace(L"protectThreshold", getProtectThreshold().value().ToString());
		}

		return dict;
	}
}
