#include <GetConfigRequest.h>

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring GetConfigRequest::getTenant() const
	{
		return Tenant;
	}

	void GetConfigRequest::setTenant(const std::wstring &value)
	{
		Tenant = value;
	}

	std::wstring GetConfigRequest::getDataId() const
	{
		return DataId;
	}

	void GetConfigRequest::setDataId(const std::wstring &value)
	{
		DataId = value;
	}

	std::wstring GetConfigRequest::getGroup() const
	{
		return Group;
	}

	void GetConfigRequest::setGroup(const std::wstring &value)
	{
		Group = value;
	}

	void GetConfigRequest::CheckParam()
	{
		ParamUtil::CheckKeyParam(getDataId(), getGroup());
	}

	std::unordered_map<std::wstring, std::wstring> GetConfigRequest::ToDict()
	{
		auto dict = std::unordered_map<std::wstring, std::wstring>
		{
			{"dataId", getDataId()},
			{"group", getGroup()}
		};

		if (!StringHelper::isEmptyOrWhiteSpace(getTenant()))
		{
			dict.emplace("tenant", getTenant());
		}

		return dict;
	}
}
