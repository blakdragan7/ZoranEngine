#pragma once
#include <ZGI\Windows\ZGIVirtualWindow.h>

class ZGITreePanel;
class ZGIGridPanel;
class ZGILabel;
class ZoranEngine_EXPORT ZGIDebugWindow : public ZGIVirtualWindow
{
private:
	ZGITreePanel * tree;
	ZGIGridPanel * grid;
	ZGILabel* fpsLabel;

public:
	ZGIDebugWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, ZGIVirtualWindow* parent = 0);
	~ZGIDebugWindow();

	void SetFPS(double fps);
	inline ZGITreePanel* GetTree()const { return tree; }
};

