#pragma once
#include <ZGI/Windows/ZGIVirtualWindow.h>

class ZGITabPanel;
class ZGIGameView;
class ZoranEditorMainWindow : public ZGIVirtualWindow
{
private:
	ZGITabPanel* mainPanel;
	ZGIGameView* GameView;

public:
	ZoranEditorMainWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, ZGIVirtualWindow* parent = 0);
	~ZoranEditorMainWindow();
};

