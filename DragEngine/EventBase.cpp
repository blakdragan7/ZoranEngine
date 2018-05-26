#include "stdafx.h"
#include "EventBase.h"


EventBase::EventBase(EventHandler object)
{
	this->object = object;
}


EventBase::~EventBase()
{
}
