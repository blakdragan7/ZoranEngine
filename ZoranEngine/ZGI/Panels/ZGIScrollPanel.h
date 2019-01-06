#pragma once
#include <ZGI\Panels\ZGIPanel.h>

class ZGIVirtualViewport;
class ZGIScrollBar;
class ZoranEngine_EXPORT ZGIScrollPanel : public ZGIPanel
{
private:
	ZGIWidget * content;

	ZGIVirtualViewport* viewport;
	Vector2D scrollOffset;

	Vector2D scrollDirection;
	bool isScrolling;
	bool canScrollVertical, canScrollHorizontal;

	ZGIScrollBar* hScrollBar;
	ZGIScrollBar* vScrollBar;

	bool scrollNeedsUpdate;

private:
	void SizeAndPositionScrollBar();

public:
	ZGIScrollPanel(ZGIVirtualWindow* owner);
	~ZGIScrollPanel();

	Vec2D GetScrollOffset();

	/* Pannel Override */

	virtual void AnimateAllWidgets(float dt)override;

	virtual bool ContainsWidget(ZGIWidget* widget)const override;
	virtual bool CanAddWidget(ZGIWidget* widget)const override;

	/* This assumes CanAddWidget was already called. Calling this in release mode without first checking CanAddWidget can result in a memory leak. */
	void AddWidget(ZGIWidget* widget);
	void RemoveWidget(ZGIWidget* widget);

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	virtual ZGIWidget* WidgetForPosition(Vec2D pos)override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual void SetSize(Vec2D size)override;
	virtual void SetPosition(Vec2D position)override;

	virtual void Render(const Matrix44& projection)override;
	virtual ZGIWidget* HitTest(Vec2D pos)override;

	virtual const char* GetClassString()const override { return "ZGIScrollPanel"; }
	virtual void Print(unsigned tabs)const override;

	// mouse event
	virtual bool MouseMove(const PlatformMouseBase& mouse)override;
	virtual bool MouseScroll(const PlatformMouseBase& mouse, float scrollAmount)override;

	// keyboard event
	virtual bool RawKeyEvent(KeyEventType type, unsigned key);

	
};
