#pragma once

#include <string>
#include <unordered_map>
#include "stringhelper.h"

namespace Nacos
{
	using namespace Nacos::Utilities;

	class GetServiceRequest : public BaseRequest
	{
	private:
		std::wstring ServiceName;
		std::wstring NamespaceId;
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
		/// group name
		/// </summary>
		std::wstring getGroupName() const;
		void setGroupName(const std::wstring &value);

		void CheckParam() override;

		std::unordered_map<std::wstring, std::wstring> ToDict() override;
	};
}
