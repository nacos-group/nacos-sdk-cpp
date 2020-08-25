#pragma once

#include <string>

namespace Nacos
{

	class Listener
	{
	private:
		std::wstring Name;
		Timer *Timer;

	public:
		Listener(const std::wstring &name, Timer *timer);

		std::wstring getName() const;
		void setName(const std::wstring &value);
		Timer *getTimer() const;
		void setTimer(Timer *value);
	};
}
