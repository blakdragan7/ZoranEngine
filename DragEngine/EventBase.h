#pragma once
#include "CommomTypes.h"
#include "EventTypes.h"

class DRAGENGINE_EXPORT EventBase
{
private:
	EventHandler object;
	unsigned EventType;

public:
	EventBase(EventHandler object);
	virtual ~EventBase();

	virtual bool DispatchEvent() = 0;
	virtual void UpdateEvent(double deltaTime) = 0;
	virtual bool ShouldDispatchEvent() = 0;
};
