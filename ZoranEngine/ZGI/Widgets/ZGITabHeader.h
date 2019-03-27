#pragma once

#include <ZGI/Widgets/ZGIWidget.h>
#include <Rendering/Primitives.h>
#include <Resources/FontResource.h>

class ZGILabelButton;
class ZGITabHeader : public ZGIWidget
{
private:
	ZGILabelButton* button;

	Color selectedColor;
	Color notSelectedColor;

	bool isActive;

public:
	ZGITabHeader(std::string& headerText, ZGIVirtualWindow* owningWindow);
	ZGITabHeader(std::string& headerText, FontResource font, ZGIVirtualWindow* owningWindow);
	~ZGITabHeader();

	void SetHeaderActive(bool active);

	// widget override

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual ZGIWidget* HitTest(Vec2D pos)override;

	virtual const char* GetClassString()const override { return "ZGIButton"; }

	virtual void Render(const Matrix44& projection)override;
};

