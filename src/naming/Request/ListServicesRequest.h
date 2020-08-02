#pragma once

#include <string>
#include <unordered_map>
#include "stringhelper.h"

namespace Nacos
{

	class ListServicesRequest : public BaseRequest
	{
	private:
		int PageNo = 0;
		int PageSize = 0;
		std::wstring NamespaceId;
		std::wstring GroupName;

		/// <summary>
		/// current page number
		/// </summary>
	public:
		int getPageNo() const;
		void setPageNo(int value);

		/// <summary>
		/// page size
		/// </summary>
		int getPageSize() const;
		void setPageSize(int value);

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
