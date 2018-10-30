#pragma once
#include <ZGI\Widgets\ZGIWidget.h>

class ZoranEngine_EXPORT ZGIPanel : public ZGIWidget
{
public:
	ZGIPanel();
	~ZGIPanel();

	virtual bool CanAddWidget(ZGIWidget* widget)const = 0;
	virtual void AddWidget(ZGIWidget* widget) = 0;
	virtual void RemoveWidget(ZGIWidget* widget) = 0;

	virtual int GetNumberOfWidgets()const = 0;
	virtual int GetMaxNumberOfWidgets()const = 0;

	// Generally, calls hittest on every widget with pos and returns the first to return true or 0 if none do
	virtual ZGIWidget* WidgetForPosition(Vec2D pos) = 0;
};
