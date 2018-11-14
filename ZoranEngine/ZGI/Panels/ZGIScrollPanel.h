#pragma once
#include <ZGI\Panels\ZGIPanel.h>

class GUIColoredRectRenderer;
class ZGIVirtualViewport;
class ZoranEngine_EXPORT ZGIScrollPanel : public ZGIPanel
{
private:
	ZGIWidget * content;
	GUIColoredRectRenderer* scrollBarRenderer;
	ZGIVirtualViewport* viewport;
	Vector2D scrollOffset;
	Vector2D scrollBarSize;

	Vector2D scrollAlpha;

	Vector2D scrollDirection;
	bool isScrolling;

	Matrix44 scrollBarMVP;
private:
	void SizeAndPositionScrollBar();

public:
	ZGIScrollPanel(ZGIVirtualWindow* owner);
	~ZGIScrollPanel();

	/* Pannel Override */
	virtual bool KeyEventSub(KeyEventType type, unsigned key)override;

	virtual bool CanAddWidget(ZGIWidget* widget)const override;
	/* This assumes CanAddWidget was already called. Calling this in release mode without first checking CanAddWidget can result in a memory leak. */
	virtual void AddWidget(ZGIWidget* widget)override;
	virtual void RemoveWidget(ZGIWidget* widget)override;

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	virtual ZGIWidget* WidgetForPosition(Vec2D pos)override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual void SetSize(Vec2D size)override;
	virtual void SetPosition(Vec2D position)override;

	virtual void Render(const Matrix44& projection)override;

	virtual ZGIWidget* HitTest(Vec2D pos)override;

	// mouse event
	virtual bool MouseMove(const PlatformMouseBase* mouse)override;

	// keyboard event
	virtual bool KeyEvent(KeyEventType type, unsigned key)override;
};
