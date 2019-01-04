#pragma once
#include <ZGI\Widgets\ZGIWidget.h>

class ZoranEngine_EXPORT ZGIPanel : public ZGIWidget
{

public:
	ZGIPanel(ZGIVirtualWindow* owningWindow);
	~ZGIPanel();

	virtual void AnimateAllWidgets(float dt) = 0;
	virtual void Animate(float dt)override;

	virtual bool ContainsWidget(ZGIWidget* widget)const = 0;
	virtual bool CanAddWidget(ZGIWidget* widget)const = 0;

	virtual int GetNumberOfWidgets()const = 0;
	virtual int GetMaxNumberOfWidgets()const = 0;

	// Generally, calls HitTest on every widget with pos and returns the first to return true or 0, if none do
	virtual ZGIWidget* WidgetForPosition(Vec2D pos) = 0;

	// widget override
	virtual ZGIWidget* HitTest(Vec2D pos)override;
	virtual bool MouseMove(const PlatformMouseBase* m)override;

	virtual bool MouseDown(const PlatformMouseBase*)override;
	virtual bool MouseUp(const PlatformMouseBase*)override;
};
