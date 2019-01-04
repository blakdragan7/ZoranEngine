#pragma once
#include <ZGI/Widgets/ZGIWidget.h>

class ZoranEngine_EXPORT ZGIComboBox : public ZGIWidget
{
public:
	ZGIComboBox(ZGIVirtualWindow* owningWindow);
	~ZGIComboBox();

	// Widget Overrides

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
	virtual const char* GetClassString()const override { return "ZGIComboBox"; }
};
