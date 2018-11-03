#pragma once
#include <ZGI\Windows\ZGIVirtualWindow.h>

class ZGIDebugWindow : public ZGIVirtualWindow
{
private:

public:
	ZGIDebugWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, ZGIVirtualWindow* parent = 0);
	~ZGIDebugWindow();
};

