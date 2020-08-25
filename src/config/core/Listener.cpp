#include <Listener.h>
#include<iostream>
namespace Nacos
{

	Listener::Listener(const std::wstring &name, Timer *timer)
	{
		this->setName(name);
		this->setTimer(timer);
	}

	std::wstring Listener::getName() const
	{
		return Name;
	}

	void Listener::setName(const std::wstring &value)
	{
		Name = value;
	}

	Timer *Listener::getTimer() const
	{
		return Timer;
	}

	void Listener::setTimer(Timer *value)
	{
		Timer = value;
	}
}
