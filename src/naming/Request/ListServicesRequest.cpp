#include "ListServicesRequest.h"

namespace Nacos
{

	int ListServicesRequest::getPageNo() const
	{
		return PageNo;
	}

	void ListServicesRequest::setPageNo(int value)
	{
		PageNo = value;
	}

	int ListServicesRequest::getPageSize() const
	{
		return PageSize;
	}

	void ListServicesRequest::setPageSize(int value)
	{
		PageSize = value;
	}

	std::wstring ListServicesRequest::getNamespaceId() const
	{
		return NamespaceId;
	}

	void ListServicesRequest::setNamespaceId(const std::wstring &value)
	{
		NamespaceId = value;
	}

	std::wstring ListServicesRequest::getGroupName() const
	{
		return GroupName;
	}

	void ListServicesRequest::setGroupName(const std::wstring &value)
	{
		GroupName = value;
	}

	void ListServicesRequest::CheckParam()
	{
		//return PageNo > 0 && PageSize > 0;
	}

	std::unordered_map<std::wstring, std::wstring> ListServicesRequest::ToDict()
	{
		auto dict = std::unordered_map<std::wstring, std::wstring>
		{
			{L"pageNo", std::to_wstring(getPageNo())},
			{L"pageSize", std::to_wstring(getPageSize())}
		};

		if (!StringHelper::isEmptyOrWhiteSpace(getNamespaceId()))
		{
			dict.emplace(L"namespaceId", getNamespaceId());
		}

		if (!StringHelper::isEmptyOrWhiteSpace(getGroupName()))
		{
			dict.emplace(L"groupName", getGroupName());
		}

		return dict;
	}
}
