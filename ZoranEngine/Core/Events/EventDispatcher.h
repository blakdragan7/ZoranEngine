#pragma once
#include "Core/CommomTypes.h"
#include <list>

class EventBase;
class ZoranEngine_EXPORT EventDispatcher
{
private:
	std::list<EventBase*> eventList;

public:
	EventDispatcher();
	~EventDispatcher();

	void UpdateAll(double deltaTime);
};

