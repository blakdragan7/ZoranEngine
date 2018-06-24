#pragma once
#include "Core/CommomTypes.h"

class ZoranEngine_EXPORT EventBase
{

public:
	EventBase();
	virtual ~EventBase();

	virtual bool DispatchEvent() = 0;
	virtual void UpdateEvent(double deltaTime) = 0;
	virtual bool ShouldDispatchEvent() = 0;
};