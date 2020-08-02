#pragma once

#include <string>
#include <unordered_map>
#include "stringhelper.h"

namespace Nacos
{
	using namespace Nacos::Utilities;

	class RemoveConfigRequest : public BaseRequest
	{
	private:
		std::wstring Tenant;
		std::wstring DataId;
		std::wstring Group;

		/// <summary>
		/// The tenant, corresponding to the namespace field of Nacos
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
