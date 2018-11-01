#pragma once

/*
	Keyboard Events require implementation of these events

	If any function returns true, then it captures the mouse event and the event will not continue to propogate
*/

enum KeyEventType;
class ZoranEngine_EXPORT IKeyboardEventHandler
{
public:
	virtual bool KeyEvent(KeyEventType type, unsigned key) = 0;

};