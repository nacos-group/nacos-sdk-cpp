#pragma once

#include <string>
#include <unordered_map>
#include "stringhelper.h"

namespace Nacos
{
	using namespace Nacos::Utilities;

	class PublishConfigRequest : public BaseRequest
	{
	private:
		std::wstring Tenant;
		std::wstring DataId;
		std::wstring Group;
		std::wstring Content;
		std::wstring Type;
		std::wstring AppName;
		std::wstring Tag;

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

		/// <summary>
		/// Configuration content
		/// </summary>
		std::wstring getContent() const;
		void setContent(const std::wstring &value);

		/// <summary>
		/// Configuration type, options value text, json, xml, yaml, html, properties
		/// </summary>
		std::wstring getType() const;
		void setType(const std::wstring &value);

		/// <summary>
		/// Configuration application
		/// </summary>
		std::wstring getAppName() const;
		void setAppName(const std::wstring &value);

		/// <summary>
		/// Configuration tags
		/// </summary>
		std::wstring getTag() const;
		void setTag(const std::wstring &value);

		void CheckParam() override;

		std::unordered_map<std::wstring, std::wstring> ToDict() override;
	};
}
