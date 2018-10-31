#include "stdafx.h"
#include "WindowsMouse.h"

#include <Windows.h>

WindowsMouse::WindowsMouse()
{
}

WindowsMouse::WindowsMouse(Vec2D pos) : PlatformMouseBase(pos)
{
}

WindowsMouse::~WindowsMouse()
{
}

void WindowsMouse::MoveMouse(Vec2I pos)
{
	::SetCursorPos(pos.x, pos.y);
}

void WindowsMouse::SetMouseHidden(bool isHidden)
{
	if(isHidden)while (ShowCursor(false) >= 0);
	else while (ShowCursor(true) < 0);
}
