#pragma once

#include <string>
#include <unordered_map>

namespace Nacos
{

	class BaseRequest
	{
		/// <summary>
		/// Checks whether request is valid
		/// </summary>
		/// <returns></returns>
	public:
		virtual void CheckParam() = 0;

		/// <summary>
		/// Convert request to params of API
		/// </summary>
		/// <returns></returns>
		virtual std::unordered_map<std::wstring, std::wstring> ToDict() = 0;
	};
}
