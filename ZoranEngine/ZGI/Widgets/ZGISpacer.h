#pragma once
#include <ZGI/Widgets/ZGIWidget.h>

/* This class literally does nothing but allow blank spaces between widgets like a spacer */

class ZoranEngine_EXPORT ZGISpacer : public ZGIWidget
{
public:
	ZGISpacer(ZGIVirtualWindow* window);
	~ZGISpacer();

	virtual void SetSize(Vec2D size)override;

	// widget overides

	virtual const char* GetClassString()const override { return "ZGISpacer"; };
	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
};

