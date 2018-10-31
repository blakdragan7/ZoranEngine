#pragma once
#include <Core\PlatformMouseBase.h>

class WindowsMouse : public PlatformMouseBase
{
public:
	WindowsMouse();
	WindowsMouse(Vec2D);
	~WindowsMouse();

	virtual void MoveMouse(Vec2I pos)override;
	virtual void SetMouseHidden(bool isHidden)override;
};

