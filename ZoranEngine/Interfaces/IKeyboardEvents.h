#pragma once

enum KeyEventType;
class ZoranEngine_EXPORT IKeyboardEvents
{
public:
	virtual void KeyEvent(KeyEventType type, unsigned key) {};

};