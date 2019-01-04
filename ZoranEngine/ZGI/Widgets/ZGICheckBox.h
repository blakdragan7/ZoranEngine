#pragma once
#include <ZGI/Widgets/ZGIWidget.h>

class ZoranEngine_EXPORT ZGICheckBox : public ZGIWidget
{
public:
	ZGICheckBox(ZGIVirtualWindow* owningWindow);
	~ZGICheckBox();

	// Widget Overrides

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
	virtual const char* GetClassString()const override { return "ZGICheckBox"; }
};
