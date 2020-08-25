#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include "stringhelper.h"

namespace Nacos
{
	using namespace Nacos::Utilities;

	class ListInstancesRequest : public BaseRequest
	{
	private:
		std::wstring ServiceName;
		std::wstring NamespaceId;
		std::wstring Clusters;
		std::wstring GroupName;
		std::optional<bool> HealthyOnly;

		/// <summary>
		/// Service name
		/// </summary>
	public:
		std::wstring getServiceName() const;
		void setServiceName(const std::wstring &value);

		/// <summary>
		/// ID of namespace
		/// </summary>
		std::wstring getNamespaceId() const;
		void setNamespaceId(const std::wstring &value);

		/// <summary>
		/// Cluster name, splited by comma
		/// </summary>
		std::wstring getClusters() const;
		void setClusters(const std::wstring &value);

		/// <summary>
		/// group name
		/// </summary>
		std::wstring getGroupName() const;
		void setGroupName(const std::wstring &value);

		/// <summary>
		/// Return healthy instance or not
		/// </summary>
		std::optional<bool> getHealthyOnly() const;
		void setHealthyOnly(const std::optional<bool> &value);

		void CheckParam() override;

		std::unordered_map<std::wstring, std::wstring> ToDict() override;
	};
}
