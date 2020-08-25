#pragma once

#include <string>

namespace Nacos
{
	class ConstValue final
	{
		/// <summary>
		/// default httpclient name
		/// </summary>
	public:
		static const std::wstring ClientName;

		/// <summary>
		/// nacos csharp client version
		/// </summary>
		static const std::wstring ClientVersion;

		/// <summary>
		/// nacos request module
		/// </summary>
		static const std::wstring RequestModule;

		/// <summary>
		/// default group 
		/// </summary>
		static const std::wstring DefaultGroup;

		/// <summary>
		/// default long pulling timeout
		/// </summary>
		static constexpr int LongPullingTimeout = 30;

		/// <summary>
		/// invalid param
		/// </summary>
		static constexpr int CLIENT_INVALID_PARAM = -400;

		/// <summary>
		/// over client threshold
		/// </summary>
		static constexpr int CLIENT_OVER_THRESHOLD = -503;

		/// <summary>
		/// invalid param
		/// </summary>
		static constexpr int INVALID_PARAM = 400;

		/// <summary>
		/// no right
		/// </summary>
		static constexpr int NO_RIGHT = 403;

		/// <summary>
		///  not found
		/// </summary>
		static constexpr int NOT_FOUND = 404;

		/// <summary>
		/// conflict
		/// </summary>
		static constexpr int CONFLICT = 409;

		/// <summary>
		/// conflict
		/// </summary>
		static constexpr int SERVER_ERROR = 500;

		/// <summary>
		/// bad gateway
		/// </summary>
		static constexpr int BAD_GATEWAY = 502;

		/// <summary>
		/// over threshold
		/// </summary>
		static constexpr int OVER_THRESHOLD = 503;

		static std::wstring ENV_LIST_KEY;

		static std::wstring ALL_IPS;

		static std::wstring FAILOVER_SWITCH;

		static std::wstring DEFAULT_NAMESPACE_ID;

		static int REQUEST_DOMAIN_RETRY_COUNT;

		static std::wstring SERVER_ADDR_IP_SPLITER;

		static std::wstring HTTP;

		static std::wstring HTTPS;

		static std::wstring HTTP_PREFIX;

		static std::wstring ACCESS_TOKEN;

		static std::wstring TOKEN_TTL;

	};
}
