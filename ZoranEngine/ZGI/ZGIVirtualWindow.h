#pragma once
#include <vector>

#include <ZGI/ZGIBase.h>

class ZGIVirtualViewport;
class ZGIWidget;
class ZoranEngine_EXPORT ZGIVirtualWindow : public ZGIBase
{
private:
	ZGIVirtualViewport * viewport;
	std::vector<ZGIVirtualWindow*>* subWindows;
	ZGIVirtualWindow* parent;

	ZGIWidget* rootContent;

public:
	ZGIVirtualWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, ZGIVirtualWindow* parent = 0);
	virtual ~ZGIVirtualWindow();
	
	virtual void OSWindowWasResized(Vec2I newSize);

	virtual void SetRootContent(ZGIWidget* widget);
	inline ZGIWidget* GetRootContent() { return rootContent; };

	void AddSubWindow(ZGIVirtualWindow* subWindow);
	void RemoveSubWindow(ZGIVirtualWindow* subWindow);
	
	void SetWindowPosition(Vec2D newPosition);
	void ResizeVirtualWindow(Vec2D newSize);

	Vec2D GetWindowSize()const;
	Vec2D GetWindowPosition()const;

	virtual void RenderWindow(Vec2D globalOffset);

	inline ZGIVirtualViewport* GetViewport() { return viewport; }
};

