#pragma once

/*
	Mouse Events require implementation of these events
*/

class PlatformMouseBase;
class ZoranEngine_EXPORT IMouseEventHandler
{
public:
	virtual void MouseDown(const PlatformMouseBase*) = 0;
	virtual void MouseMove(const PlatformMouseBase*) = 0;
	virtual void MouseUp(const PlatformMouseBase*) = 0;

	// mouse
	virtual void MouseEnterd(const PlatformMouseBase*) = 0;
	virtual void MouseLeft(const PlatformMouseBase*) = 0;
};