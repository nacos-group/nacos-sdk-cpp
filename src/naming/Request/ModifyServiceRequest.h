#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include "stringhelper.h"

namespace Nacos
{
	using namespace Nacos::Utilities;

	class ModifyServiceRequest : public BaseRequest
	{
	private:
		std::wstring ServiceName;
		std::wstring NamespaceId;
		std::optional<float> ProtectThreshold;
		std::wstring Selector;
		std::wstring Metadata;
		std::wstring GroupName;

		/// <summary>
		/// service name
		/// </summary>
	public:
		std::wstring getServiceName() const;
		void setServiceName(const std::wstring &value);

		/// <summary>
		/// namespace id
		/// </summary>
		std::wstring getNamespaceId() const;
		void setNamespaceId(const std::wstring &value);

		/// <summary>
		/// Protect threshold, set value from 0 to 1, default 0
		/// </summary>
		std::optional<float> getProtectThreshold() const;
		void setProtectThreshold(const std::optional<float> &value);

		/// <summary>
		/// visit strategy, a JSON string
		/// </summary>
		std::wstring getSelector() const;
		void setSelector(const std::wstring &value);

		/// <summary>
		/// metadata of service
		/// </summary>
		std::wstring getMetadata() const;
		void setMetadata(const std::wstring &value);

		/// <summary>
		/// group name
		/// </summary>
		std::wstring getGroupName() const;
		void setGroupName(const std::wstring &value);

		void CheckParam() override;

		std::unordered_map<std::wstring, std::wstring> ToDict() override;
	};
}
