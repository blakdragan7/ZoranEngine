#pragma once
#include "Core/CommomTypes.h"
#include <list>

class EventBase;
class ZoranEngine_EXPORT EventDispatcher
{
private:
#pragma warning(push)
#pragma warning(disable:4251)
	std::list<EventBase*> eventList;
#pragma warning(pop)

public:
	EventDispatcher();
	~EventDispatcher();

	void UpdateAll(float deltaTime);
};

