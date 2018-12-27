#pragma once

/*
	Keyboard Events require implementation of these events

	If any function returns true, then it captures the mouse event and the event will not continue to propogate
*/

enum KeyEventType;
class ZoranEngine_EXPORT IKeyboardEventHandler
{
public:
	bool isInFocus; 

public:
	IKeyboardEventHandler() : isInFocus(false) {}
	// This is the raw event from the ekyboard before processing
	virtual bool RawKeyEvent(KeyEventType type, unsigned rawKey) = 0;
	// this is the transalted event from the keyboard (scanecode => unicode based on keyboard and control keys)
	virtual bool CharEvent(unsigned uni) = 0;
};