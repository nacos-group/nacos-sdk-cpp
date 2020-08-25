#include <SendHeartbeatRequest.h>

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring SendHeartbeatRequest::getServiceName() const
	{
		return ServiceName;
	}

	void SendHeartbeatRequest::setServiceName(const std::wstring &value)
	{
		ServiceName = value;
	}

	std::wstring SendHeartbeatRequest::getBeat() const
	{
		return getBeatInfo()->ToJsonString();
	}

	BeatInfo *SendHeartbeatRequest::getBeatInfo() const
	{
		return BeatInfo;
	}

	void SendHeartbeatRequest::setBeatInfo(BeatInfo *value)
	{
		BeatInfo = value;
	}

	std::wstring SendHeartbeatRequest::getGroupName() const
	{
		return GroupName;
	}

	void SendHeartbeatRequest::setGroupName(const std::wstring &value)
	{
		GroupName = value;
	}

	std::optional<bool> SendHeartbeatRequest::getEphemeral() const
	{
		return Ephemeral;
	}

	void SendHeartbeatRequest::setEphemeral(const std::optional<bool> &value)
	{
		Ephemeral = value;
	}

	std::wstring SendHeartbeatRequest::getNameSpaceId() const
	{
		return NameSpaceId;
	}

	void SendHeartbeatRequest::setNameSpaceId(const std::wstring &value)
	{
		NameSpaceId = value;
	}

	void SendHeartbeatRequest::CheckParam()
	{
		return BeatInfo != null && !string.IsNullOrWhiteSpace(ServiceName);
	}

	std::unordered_map<std::wstring, std::wstring> SendHeartbeatRequest::ToDict()
	{
		auto dict = std::unordered_map<std::wstring, std::wstring>
		{
			{"serviceName", getServiceName()},
			{"beat", getBeat()}
		};

		if (!StringHelper::isEmptyOrWhiteSpace(getNameSpaceId()))
		{
			dict.emplace("namespaceId", getNameSpaceId());
		}

		if (getEphemeral())
		{

			dict.emplace("ephemeral", getEphemeral().value().ToString());
		}


		if (!StringHelper::isEmptyOrWhiteSpace(getGroupName()))
		{
			dict.emplace("groupName", getGroupName());
		}

		return dict;
	}
}
