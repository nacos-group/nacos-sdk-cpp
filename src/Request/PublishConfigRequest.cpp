#include <PublishConfigRequest.h>

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring PublishConfigRequest::getTenant() const
	{
		return Tenant;
	}

	void PublishConfigRequest::setTenant(const std::wstring &value)
	{
		Tenant = value;
	}

	std::wstring PublishConfigRequest::getDataId() const
	{
		return DataId;
	}

	void PublishConfigRequest::setDataId(const std::wstring &value)
	{
		DataId = value;
	}

	std::wstring PublishConfigRequest::getGroup() const
	{
		return Group;
	}

	void PublishConfigRequest::setGroup(const std::wstring &value)
	{
		Group = value;
	}

	std::wstring PublishConfigRequest::getContent() const
	{
		return Content;
	}

	void PublishConfigRequest::setContent(const std::wstring &value)
	{
		Content = value;
	}

	std::wstring PublishConfigRequest::getType() const
	{
		return Type;
	}

	void PublishConfigRequest::setType(const std::wstring &value)
	{
		Type = value;
	}

	std::wstring PublishConfigRequest::getAppName() const
	{
		return AppName;
	}

	void PublishConfigRequest::setAppName(const std::wstring &value)
	{
		AppName = value;
	}

	std::wstring PublishConfigRequest::getTag() const
	{
		return Tag;
	}

	void PublishConfigRequest::setTag(const std::wstring &value)
	{
		Tag = value;
	}

	void PublishConfigRequest::CheckParam()
	{
		ParamUtil::CheckParam(getDataId(), getGroup(), getContent());
	}

	std::unordered_map<std::wstring, std::wstring> PublishConfigRequest::ToDict()
	{
		auto dict = std::unordered_map<std::wstring, std::wstring>
		{
			{"dataId", getDataId()},
			{"group", getGroup()},
			{"content", getContent()}
		};

		if (!StringHelper::isEmptyOrWhiteSpace(getTenant()))
		{
			dict.emplace("tenant", getTenant());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getType()))
		{
			dict.emplace("type", getType());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getAppName()))
		{
			dict.emplace("appName", getAppName());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getTag()))
		{
			dict.emplace("tag", getTag());
		}

		return dict;
	}
}
