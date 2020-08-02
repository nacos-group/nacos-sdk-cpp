#include <CreateServiceRequest.h>

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring CreateServiceRequest::getServiceName() const
	{
		return ServiceName;
	}

	void CreateServiceRequest::setServiceName(const std::wstring &value)
	{
		ServiceName = value;
	}

	std::wstring CreateServiceRequest::getNamespaceId() const
	{
		return NamespaceId;
	}

	void CreateServiceRequest::setNamespaceId(const std::wstring &value)
	{
		NamespaceId = value;
	}

	std::optional<float> CreateServiceRequest::getProtectThreshold() const
	{
		return ProtectThreshold;
	}

	void CreateServiceRequest::setProtectThreshold(const std::optional<float> &value)
	{
		ProtectThreshold = value;
	}

	std::wstring CreateServiceRequest::getSelector() const
	{
		return Selector;
	}

	void CreateServiceRequest::setSelector(const std::wstring &value)
	{
		Selector = value;
	}

	std::wstring CreateServiceRequest::getMetadata() const
	{
		return Metadata;
	}

	void CreateServiceRequest::setMetadata(const std::wstring &value)
	{
		Metadata = value;
	}

	std::wstring CreateServiceRequest::getGroupName() const
	{
		return GroupName;
	}

	void CreateServiceRequest::setGroupName(const std::wstring &value)
	{
		GroupName = value;
	}

	void CreateServiceRequest::CheckParam()
	{
		ParamUtil::CheckServiceName(getServiceName());
	}

	std::unordered_map<std::wstring, std::wstring> CreateServiceRequest::ToDict()
	{
		auto dict = std::unordered_map<std::wstring, std::wstring>
		{
			{"serviceName", getServiceName()}
		};

		if (!StringHelper::isEmptyOrWhiteSpace(getNamespaceId()))
		{
			dict.emplace("namespaceId", getNamespaceId());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getMetadata()))
		{
			dict.emplace("metadata", getMetadata());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getGroupName()))
		{
			dict.emplace("groupName", getGroupName());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getSelector()))
		{
			dict.emplace("selector", getSelector());
		}

		if (getProtectThreshold())
		{

			dict.emplace("protectThreshold", getProtectThreshold().ToString());
		}

		return dict;
	}
}
