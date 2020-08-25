#include <RemoveListenerRequest.h>

namespace Nacos
{
	using namespace Nacos::Utilities;

	std::wstring RemoveListenerRequest::getTenant() const
	{
		return Tenant;
	}

	void RemoveListenerRequest::setTenant(const std::wstring &value)
	{
		Tenant = value;
	}

	std::wstring RemoveListenerRequest::getDataId() const
	{
		return DataId;
	}

	void RemoveListenerRequest::setDataId(const std::wstring &value)
	{
		DataId = value;
	}

	std::wstring RemoveListenerRequest::getGroup() const
	{
		return Group;
	}

	void RemoveListenerRequest::setGroup(const std::wstring &value)
	{
		Group = value;
	}

	std::vector<std::function<void()>> RemoveListenerRequest::getCallbacks() const
	{
		return Callbacks;
	}

	void RemoveListenerRequest::setCallbacks(const std::vector<std::function<void()>> &value)
	{
		Callbacks = value;
	}

	void RemoveListenerRequest::CheckParam()
	{
		ParamUtil::CheckTDG(getTenant(), getDataId(), getGroup());
	}

	std::unordered_map<std::wstring, std::wstring> RemoveListenerRequest::ToDict()
	{
		return std::unordered_map<std::wstring, std::wstring>();
	}
}
