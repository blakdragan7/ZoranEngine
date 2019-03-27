#pragma once
#include <ZGI/Windows/ZGIVirtualWindow.h>

class ZGIVirtualGameWindow;
class ZoranEditorMainWindow : public ZGIVirtualWindow
{
private:
	ZGIVirtualGameWindow* GameView;

public:
	ZoranEditorMainWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, ZGIVirtualWindow* parent = 0);
	~ZoranEditorMainWindow();
};

