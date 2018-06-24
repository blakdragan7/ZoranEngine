#include "stdafx.h"
#include "EventDispatcher.h"
#include "EventBase.h"

EventDispatcher::EventDispatcher()
{
}


EventDispatcher::~EventDispatcher()
{
}

void EventDispatcher::UpdateAll(double deltaTime)
{
	std::list<EventBase*>::iterator i = eventList.begin();

	while (i != eventList.end())
	{
		(*i)->UpdateEvent(deltaTime);

		if ((*i)->ShouldDispatchEvent())
		{
			if ((*i)->DispatchEvent())
			{
				eventList.erase(i++);
				continue;
			}
		}

		i++;
	}
}
