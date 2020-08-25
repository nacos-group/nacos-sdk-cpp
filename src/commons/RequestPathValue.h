#pragma once

#include <string>

namespace Nacos
{
	class RequestPathValue final
	{
	public:
		static const std::wstring CONFIGS;
		static const std::wstring CONFIGS_LISTENER;

		static const std::wstring INSTANCE;
		static const std::wstring INSTANCE_LIST;
		static const std::wstring INSTANCE_BEAT;
		static const std::wstring INSTANCE_HEALTH;

		static const std::wstring SERVICE;
		static const std::wstring SERVICE_LIST;

		static const std::wstring SWITCHES;

		static const std::wstring SERVERS;

		static const std::wstring LEADER;

		static const std::wstring METRICS;
	};
}
