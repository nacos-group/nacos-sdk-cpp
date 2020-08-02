#pragma once

#include <string>
#include <unordered_map>
#include <optional>

namespace Nacos
{

	class ListClusterServersRequest : public BaseRequest
	{
	private:
		std::optional<bool> Healthy;

		/// <summary>
		/// if return healthy servers only
		/// </summary>
	public:
		std::optional<bool> getHealthy() const;
		void setHealthy(const std::optional<bool> &value);

		void CheckParam() override;

		std::unordered_map<std::wstring, std::wstring> ToDict() override;
	};
}
