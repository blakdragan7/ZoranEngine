#pragma once
#include <ZGI/Widgets/ZGIWidget.h>

class ZoranEngine_EXPORT ZGISpinBox : public ZGIWidget
{
public:
	ZGISpinBox(ZGIVirtualWindow* owningWindow);
	~ZGISpinBox();

	// Widget Overrides

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
	virtual const char* GetClassString()const override { return "ZGISpinBox"; }
};
