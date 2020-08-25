#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include "stringhelper.h"

namespace Nacos
{
	using namespace Nacos::Utilities;

	class SendHeartbeatRequest : public BaseRequest
	{
	private:
		std::wstring ServiceName;
		BeatInfo *BeatInfo;
		std::wstring GroupName;
		std::optional<bool> Ephemeral;
		std::wstring NameSpaceId;

		/// <summary>
		/// Service Name
		/// </summary>
	public:
		std::wstring getServiceName() const;
		void setServiceName(const std::wstring &value);

		/// <summary>
		/// beat content
		/// </summary>
		std::wstring getBeat() const;

		/// <summary>
		/// beat info
		/// </summary>
		BeatInfo *getBeatInfo() const;
		void setBeatInfo(BeatInfo *value);

		/// <summary>
		/// group name
		/// </summary>
		std::wstring getGroupName() const;
		void setGroupName(const std::wstring &value);

		/// <summary>
		/// if instance is ephemeral
		/// </summary>
		std::optional<bool> getEphemeral() const;
		void setEphemeral(const std::optional<bool> &value);

		/// <summary>
		/// namespace id
		/// </summary>
		std::wstring getNameSpaceId() const;
		void setNameSpaceId(const std::wstring &value);

		void CheckParam() override;

		std::unordered_map<std::wstring, std::wstring> ToDict() override;
	};
}
