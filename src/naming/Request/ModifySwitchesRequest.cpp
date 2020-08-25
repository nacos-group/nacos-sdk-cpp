#include "ModifySwitchesRequest.h"

namespace Nacos
{

	std::wstring ModifySwitchesRequest::getEntry() const
	{
		return Entry;
	}

	void ModifySwitchesRequest::setEntry(const std::wstring &value)
	{
		Entry = value;
	}

	std::wstring ModifySwitchesRequest::getValue() const
	{
		return Value;
	}

	void ModifySwitchesRequest::setValue(const std::wstring &value)
	{
		Value = value;
	}

	std::optional<bool> ModifySwitchesRequest::getDebug() const
	{
		return Debug;
	}

	void ModifySwitchesRequest::setDebug(const std::optional<bool> &value)
	{
		Debug = value;
	}

	void ModifySwitchesRequest::CheckParam()
	{
		return (!string.IsNullOrWhiteSpace(Entry) && !string.IsNullOrWhiteSpace(Value));
	}

	std::unordered_map<std::wstring, std::wstring> ModifySwitchesRequest::ToDict()
	{
		auto dict = std::unordered_map<std::wstring, std::wstring>
		{
			{"entry", getEntry()},
			{"value", getValue()}
		};

		if (getDebug())
		{

			dict.emplace("debug", getDebug().value().ToString());
		}

		return dict;
	}
}
