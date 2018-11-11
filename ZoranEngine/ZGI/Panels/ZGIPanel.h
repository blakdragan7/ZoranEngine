#pragma once
#include <ZGI\Widgets\ZGIWidget.h>

class ZoranEngine_EXPORT ZGIPanel : public ZGIWidget
{

public:
	ZGIPanel(ZGIVirtualWindow* owningWindow);
	~ZGIPanel();


	virtual bool KeyEventSub(KeyEventType type, unsigned key) = 0;

	virtual bool CanAddWidget(ZGIWidget* widget)const = 0;
	virtual void AddWidget(ZGIWidget* widget) = 0;
	virtual void RemoveWidget(ZGIWidget* widget) = 0;

	virtual int GetNumberOfWidgets()const = 0;
	virtual int GetMaxNumberOfWidgets()const = 0;

	// Generally, calls hittest on every widget with pos and returns the first to return true or 0 if none do
	virtual ZGIWidget* WidgetForPosition(Vec2D pos) = 0;

	// widget override
	virtual ZGIWidget* HitTest(Vec2D pos)override;
	virtual bool MouseMove(const PlatformMouseBase* m)override;

	virtual bool MouseDown(const PlatformMouseBase*)override;
	virtual bool MouseUp(const PlatformMouseBase*)override;

	virtual bool KeyEvent(KeyEventType type, unsigned key)override;
};
