#include <ListenerConfigRequest.h>

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring ListenerConfigRequest::getDataId() const
	{
		return DataId;
	}

	void ListenerConfigRequest::setDataId(const std::wstring &value)
	{
		DataId = value;
	}

	std::wstring ListenerConfigRequest::getGroup() const
	{
		return Group;
	}

	void ListenerConfigRequest::setGroup(const std::wstring &value)
	{
		Group = value;
	}

	std::wstring ListenerConfigRequest::getContentMD5() const
	{
		return HashUtil::GetMd5(getContent());
	}

	std::wstring ListenerConfigRequest::getTenant() const
	{
		return Tenant;
	}

	void ListenerConfigRequest::setTenant(const std::wstring &value)
	{
		Tenant = value;
	}

	std::wstring ListenerConfigRequest::getContent() const
	{
		return Content;
	}

	void ListenerConfigRequest::setContent(const std::wstring &value)
	{
		Content = value;
	}

	std::wstring ListenerConfigRequest::getListeningConfigs() const
	{
		return StringHelper::isEmptyOrWhiteSpace(getTenant()) ? std::wstring::Format("{0}{1}{2}{3}{4}{5}", getDataId(), CharacterUtil::TwoEncode, getGroup(), CharacterUtil::TwoEncode, getContentMD5(), CharacterUtil::OneEncode) : std::wstring::Format(L"{0}{1}{2}{3}{4}{5}{6}{7}", getDataId(), CharacterUtil::TwoEncode, getGroup(), CharacterUtil::TwoEncode, getContentMD5(), CharacterUtil::TwoEncode, getTenant(), CharacterUtil::OneEncode);
	}

	void ListenerConfigRequest::CheckParam()
	{
		return !string.IsNullOrWhiteSpace(DataId);
	}

	std::unordered_map<std::wstring, std::wstring> ListenerConfigRequest::ToDict()
	{
		return std::unordered_map<std::wstring, std::wstring>
		{
			{"Listening-Configs", getListeningConfigs()}
		};
	}
}
