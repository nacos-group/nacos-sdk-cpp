#include <ListClusterServersRequest.h>

namespace Nacos
{

	std::optional<bool> ListClusterServersRequest::getHealthy() const
	{
		return Healthy;
	}

	void ListClusterServersRequest::setHealthy(const std::optional<bool> &value)
	{
		Healthy = value;
	}

	void ListClusterServersRequest::CheckParam()
	{
		//return true;
	}

	std::unordered_map<std::wstring, std::wstring> ListClusterServersRequest::ToDict()
	{
		auto dict = std::unordered_map<std::wstring, std::wstring>();

		if (getHealthy())
		{

			dict.emplace("healthy", getHealthy().value().ToString());
		}

		return dict;
	}
}
