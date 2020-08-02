#pragma once

#include <string>
#include <unordered_map>
#include "stringhelper.h"

namespace Nacos
{
	using namespace Nacos::Utilities;

	class ListenerConfigRequest : public BaseRequest
	{
	private:
		std::wstring DataId;
		std::wstring Group;
		std::wstring Tenant;
		std::wstring Content;

		/// <summary>
		/// A packet field indicating the configuration ID.
		/// </summary>
	public:
		std::wstring getDataId() const;
		void setDataId(const std::wstring &value);

		/// <summary>
		/// A packet field indicating the configuration group.
		/// </summary>
		std::wstring getGroup() const;
		void setGroup(const std::wstring &value);

		/// <summary>
		/// A packet field indicating the MD5 value of the configuration.
		/// </summary>
		std::wstring getContentMD5() const;

		/// <summary>
		/// A packet field indicating tenant information. It corresponds to the Namespace field in Nacos.
		/// </summary>
		std::wstring getTenant() const;
		void setTenant(const std::wstring &value);

		/// <summary>
		/// The configuration value
		/// </summary>
		std::wstring getContent() const;
		void setContent(const std::wstring &value);

		/// <summary>
		/// A request to listen for data packets
		/// </summary>
		std::wstring getListeningConfigs() const;


		void CheckParam() override;

		std::unordered_map<std::wstring, std::wstring> ToDict() override;
	};
}
