#pragma once

/*
	Mouse Events require implementation of these events
*/

class MouseInstance;
class ZoranEngine_EXPORT IMouseEventHandler
{
public:
	virtual void MouseDown(const MouseInstance&) = 0;
	virtual void MouseMove(const MouseInstance&) = 0;
	virtual void MouseUp(const MouseInstance&) = 0;

	// mouse
	virtual void MouseEnterd(const MouseInstance&) = 0;
	virtual void MouseLeft(const MouseInstance&) = 0;
};