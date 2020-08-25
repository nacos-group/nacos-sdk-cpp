#include <AddListenerRequest.h>

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring AddListenerRequest::getTenant() const
	{
		return Tenant;
	}

	void AddListenerRequest::setTenant(const std::wstring &value)
	{
		Tenant = value;
	}

	std::wstring AddListenerRequest::getDataId() const
	{
		return DataId;
	}

	void AddListenerRequest::setDataId(const std::wstring &value)
	{
		DataId = value;
	}

	std::wstring AddListenerRequest::getGroup() const
	{
		return Group;
	}

	void AddListenerRequest::setGroup(const std::wstring &value)
	{
		Group = value;
	}

	std::vector<std::function<void(const std::wstring&)>*> AddListenerRequest::getCallbacks() const
	{
		return Callbacks;
	}

	void AddListenerRequest::setCallbacks(const std::vector<std::function<void(const std::wstring&)>*> &value)
	{
		Callbacks = value;
	}

	std::wstring AddListenerRequest::getContent() const
	{
		return Content;
	}

	void AddListenerRequest::setContent(const std::wstring &value)
	{
		Content = value;
	}

	std::wstring AddListenerRequest::getContentMD5() const
	{
		return HashUtil::GetMd5(getContent());
	}

	std::wstring AddListenerRequest::getListeningConfigs() const
	{
		return StringHelper::isEmptyOrWhiteSpace(getTenant()) ? std::wstring::Format("{0}{1}{2}{3}{4}{5}", getDataId(), CharacterUtil::TwoEncode, getGroup(), CharacterUtil::TwoEncode, getContentMD5(), CharacterUtil::OneEncode) : std::wstring::Format(L"{0}{1}{2}{3}{4}{5}{6}{7}", getDataId(), CharacterUtil::TwoEncode, getGroup(), CharacterUtil::TwoEncode, getContentMD5(), CharacterUtil::TwoEncode, getTenant(), CharacterUtil::OneEncode);
	}

	void AddListenerRequest::CheckParam()
	{
		ParamUtil::CheckTDG(getTenant(), getDataId(), getGroup());
	}

	std::unordered_map<std::wstring, std::wstring> AddListenerRequest::ToDict()
	{
		auto dict = std::unordered_map<std::wstring, std::wstring>
		{
			{"Listening-Configs", getListeningConfigs()},
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
