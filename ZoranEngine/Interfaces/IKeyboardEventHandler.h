#pragma once

enum KeyEventType;
class ZoranEngine_EXPORT IKeyboardEventHandler
{
public:
	virtual void KeyEvent(KeyEventType type, unsigned key) {};

};