#pragma once

#include <string>
#include <unordered_map>
#include "stringhelper.h"

namespace Nacos
{
	using namespace Nacos::Utilities;

	class GetConfigRequest : public BaseRequest
	{
	private:
		std::wstring Tenant;
		std::wstring DataId;
		std::wstring Group;

		/// <summary>
		/// Tenant information. It corresponds to the Namespace field in Nacos.
		/// </summary>
	public:
		std::wstring getTenant() const;
		void setTenant(const std::wstring &value);

		/// <summary>
		/// Configuration ID
		/// </summary>
		std::wstring getDataId() const;
		void setDataId(const std::wstring &value);

		/// <summary>
		/// Configuration group
		/// </summary>
		std::wstring getGroup() const;
		void setGroup(const std::wstring &value);

		void CheckParam() override;

		std::unordered_map<std::wstring, std::wstring> ToDict() override;
	};
}
