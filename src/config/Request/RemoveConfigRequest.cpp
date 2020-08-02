#include <RemoveConfigRequest.h>

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring RemoveConfigRequest::getTenant() const
	{
		return Tenant;
	}

	void RemoveConfigRequest::setTenant(const std::wstring &value)
	{
		Tenant = value;
	}

	std::wstring RemoveConfigRequest::getDataId() const
	{
		return DataId;
	}

	void RemoveConfigRequest::setDataId(const std::wstring &value)
	{
		DataId = value;
	}

	std::wstring RemoveConfigRequest::getGroup() const
	{
		return Group;
	}

	void RemoveConfigRequest::setGroup(const std::wstring &value)
	{
		Group = value;
	}

	void RemoveConfigRequest::CheckParam()
	{
		ParamUtil::CheckKeyParam(getDataId(), getGroup());
	}

	std::unordered_map<std::wstring, std::wstring> RemoveConfigRequest::ToDict()
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
