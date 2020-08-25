#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

namespace Nacos
{
	using namespace Nacos::Utilities;

	class RemoveListenerRequest : public BaseRequest
	{
	private:
		std::wstring Tenant;
		std::wstring DataId;
		std::wstring Group;
		std::vector<std::function<void()>> Callbacks = std::vector<std::function<void()>>();

		/// <summary>
		/// Tenant information. It corresponds to the Namespace field in Nacos.
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
		/// Callbacks when configuration was changed
		/// </summary>
		/// <value>The callbacks.</value>
		std::vector<std::function<void()>> getCallbacks() const;
		void setCallbacks(const std::vector<std::function<void()>> &value);

		void CheckParam() override;

		std::unordered_map<std::wstring, std::wstring> ToDict() override;
	};
}
