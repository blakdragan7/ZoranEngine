#pragma once
#include <ZGI\ZGIWidget.h>

class ZoranEngine_EXPORT ZGIPanel : public ZGIWidget
{
public:
	ZGIPanel();
	~ZGIPanel();

	virtual bool CanAddWidget(ZGIWidget* widget) = 0;
	virtual void AddWidget(ZGIWidget* widget) = 0;
	virtual void RemoveWidget(ZGIWidget* widget) = 0;
};
