#pragma once
#include "CommomTypes.h"
#include <list>

class EventBase;
class DRAGENGINE_EXPORT EventDispatcher
{
private:
	std::list<EventBase*> eventList;

public:
	EventDispatcher();
	~EventDispatcher();

	void UpdateAll(double deltaTime);
};

