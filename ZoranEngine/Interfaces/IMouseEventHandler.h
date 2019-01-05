#pragma once

/*
	Mouse Events require implementation of these events

	If any function returns true, then it captures the mouse event and the event will not continue to propogate	
*/

class PlatformMouseBase;
class ZoranEngine_EXPORT IMouseEventHandler
{
public:
	virtual bool MouseDown(const PlatformMouseBase&) = 0;
	virtual bool MouseMove(const PlatformMouseBase&) = 0;
	virtual bool MouseUp(const PlatformMouseBase&) = 0;

	virtual bool MouseEnterd(const PlatformMouseBase&) = 0;
	virtual bool MouseLeft(const PlatformMouseBase&) = 0;
};