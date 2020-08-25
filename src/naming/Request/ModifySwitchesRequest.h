#pragma once

#include <string>
#include <unordered_map>
#include <optional>

namespace Nacos
{

	class ModifySwitchesRequest : public BaseRequest
	{
	private:
		std::wstring Entry;
		std::wstring Value;
		std::optional<bool> Debug;

		/// <summary>
		/// switch name
		/// </summary>
	public:
		std::wstring getEntry() const;
		void setEntry(const std::wstring &value);

		/// <summary>
		/// switch value
		/// </summary>
		std::wstring getValue() const;
		void setValue(const std::wstring &value);

		/// <summary>
		/// if affect the local server, true means yes, false means no, default true
		/// </summary>
		std::optional<bool> getDebug() const;
		void setDebug(const std::optional<bool> &value);

		void CheckParam() override;

		std::unordered_map<std::wstring, std::wstring> ToDict() override;
	};
}
