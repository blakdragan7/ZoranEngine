#pragma once
#include <Core\PlatformMouseBase.h>

class WindowsMouse : public PlatformMouseBase
{
public:
	WindowsMouse();
	WindowsMouse(Vec2D);
	~WindowsMouse();

	virtual void SetMousePos(Vec2I pos)override;
	virtual void SetMouseHidden(bool isHidden)override;
};

