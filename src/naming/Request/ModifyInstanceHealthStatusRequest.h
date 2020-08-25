#pragma once

#include <string>
#include <unordered_map>
#include "stringhelper.h"

namespace Nacos
{
	using namespace Nacos::Utilities;

	class ModifyInstanceHealthStatusRequest : public BaseRequest
	{
	private:
		std::wstring Ip;
		int Port = 0;
		std::wstring ServiceName;
		std::wstring NamespaceId;
		bool Healthy = false;
		std::wstring ClusterName;
		std::wstring GroupName;

		/// <summary>
		/// ip of instance
		/// </summary>
	public:
		std::wstring getIp() const;
		void setIp(const std::wstring &value);

		/// <summary>
		/// port of instance
		/// </summary>
		int getPort() const;
		void setPort(int value);

		/// <summary>
		/// service name
		/// </summary>
		std::wstring getServiceName() const;
		void setServiceName(const std::wstring &value);

		/// <summary>
		/// namespace id
		/// </summary>
		std::wstring getNamespaceId() const;
		void setNamespaceId(const std::wstring &value);

		/// <summary>
		/// if healthy
		/// </summary>
		bool getHealthy() const;
		void setHealthy(bool value);

		/// <summary>
		/// cluster name
		/// </summary>
		std::wstring getClusterName() const;
		void setClusterName(const std::wstring &value);

		/// <summary>
		/// group name
		/// </summary>
		std::wstring getGroupName() const;
		void setGroupName(const std::wstring &value);

		void CheckParam() override;

		std::unordered_map<std::wstring, std::wstring> ToDict() override;
	};
}
