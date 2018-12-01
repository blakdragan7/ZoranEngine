#pragma once
#include <ZGI\Windows\ZGIVirtualWindow.h>

class ZGILabel;
class ZGIButton;
class ZoranEngine_EXPORT ZGIDebugWindow : public ZGIVirtualWindow
{
public:
	ZGIDebugWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, ZGIVirtualWindow* parent = 0);
	~ZGIDebugWindow();
};

