#pragma once
#include <vector>

#include <ZGI\Core\ZGIBase.h>
#include <Interfaces/IKeyboardEventHandler.h>
#include <Interfaces/IMouseEventHandler.h>

class ZGIVirtualViewport;
class ZGIWidget;
class ZoranEngine_EXPORT ZGIVirtualWindow : public ZGIBase, public IKeyboardEventHandler, public IMouseEventHandler
{
private:
	ZGIVirtualViewport * viewport;
	std::vector<ZGIVirtualWindow*>* subWindows;
	ZGIVirtualWindow* parent;

	ZGIWidget* rootContent;

	Vector2D globalOffsetCache;

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

	Vector2D ConvertAbsoluteToVirtual(Vec2D pos)const;

	Vec2D GetWindowSize()const;
	Vec2D GetWindowPosition()const;

	virtual void RenderWindow(Vec2D globalOffset);

	inline ZGIVirtualViewport* GetViewport() { return viewport; }

	/*IMouseEventHandler Defaults*/

	virtual bool MouseDown(const PlatformMouseBase *)override;
	virtual bool MouseMove(const PlatformMouseBase *)override;
	virtual bool MouseUp(const PlatformMouseBase *)override;

	virtual bool MouseEnterd(const PlatformMouseBase *)override;
	virtual bool MouseLeft(const PlatformMouseBase *)override;

	/*IKeyboardEvents Defaults*/

	virtual bool KeyEvent(KeyEventType type, unsigned key)override;
};

