#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include "stringhelper.h"

namespace Nacos
{
	using namespace Nacos::Utilities;

	class AddListenerRequest : public BaseRequest
	{
	private:
		std::wstring Tenant;
		std::wstring DataId;
		std::wstring Group;
		std::vector<std::function<void(const std::wstring&)>*> Callbacks = std::vector<std::function<void(const std::wstring&)>*>();
		std::wstring Content;

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
		std::vector<std::function<void(const std::wstring&)>*> getCallbacks() const;
		void setCallbacks(const std::vector<std::function<void(const std::wstring&)>*> &value);

		/// <summary>
		/// Configuraton value.
		/// </summary>
		/// <value>The content.</value>
		std::wstring getContent() const;
		void setContent(const std::wstring &value);

		/// <summary>
		/// A packet field indicating the MD5 value of the configuration.
		/// </summary>
		std::wstring getContentMD5() const;

		/// <summary>
		/// A request to listen for data packets
		/// </summary>
		std::wstring getListeningConfigs() const;


		void CheckParam() override;

		std::unordered_map<std::wstring, std::wstring> ToDict() override;
	};
}
