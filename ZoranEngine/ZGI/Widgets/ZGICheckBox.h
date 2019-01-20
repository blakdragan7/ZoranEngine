#pragma once
#include <ZGI/Widgets/ZGIWidget.h>
#include <functional>

#include <Rendering/Primitives.h>

#include <Resources/ImageResource.h>

class ZGIImage;
class ZGIHorizontalBoxPanel;
class IZGICheckBoxEventHandler;
class ZoranEngine_EXPORT ZGICheckBox : public ZGIWidget
{
private:
	ImageResource checkImage;
	ZGIWidget* content;
	ZGIHorizontalBoxPanel* panel;
	ZGIImage* image;

	bool isChecked;
	IZGICheckBoxEventHandler* handler;

	Color hoverHue;

protected:
	virtual void WasChecked(bool checked);

public:
	ZGICheckBox(ZGIVirtualWindow* owningWindow);
	ZGICheckBox(ZGIWidget* content, ZGIVirtualWindow* owningWindow);
	~ZGICheckBox();

	inline void SetHoverHue(const Color& color)
	{hoverHue = color;}

	inline void SetEventHandler(IZGICheckBoxEventHandler* handler)
	{this->handler = handler;}

	void SetChecked(bool checked);
	inline bool GetIsCheckd()const { return isChecked; }

	void SetCheckImage(const char* imagePath);
	void SetCheckImage(ImageResource image);

	void SetContent(ZGIWidget* content);

	// Widget Overrides

	virtual void Render(const Matrix44& projection)override;

	ZGIWidget* HitTest(Vec2D pos)override;

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
	virtual const char* GetClassString()const override { return "ZGICheckBox"; }

	// MouseEvents

	virtual bool MouseUp(const PlatformMouseBase&)override;
	virtual bool MouseEnterd(const PlatformMouseBase&)override;
	virtual bool MouseLeft(const PlatformMouseBase&)override;
};
