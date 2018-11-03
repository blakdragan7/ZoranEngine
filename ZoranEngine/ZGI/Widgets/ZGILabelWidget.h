#pragma once
#include<ZGI\Widgets\ZGIWidget.h>

enum VerticalTextAlignment
{
	VerticalTextAlignment_Top,
	VerticalTextAlignment_Bottom,
	VerticalTextAlignment_Center,
};

enum HorizontalTextAlignment
{
	HorizontalTextAlignment_Left,
	HorizontalTextAlignment_Right,
	HorizontalTextAlignment_Center,
};

class FontRenderer;
class FontResource;
class ZoranEngine_EXPORT ZGILabelWidget : public ZGIWidget
{
private:
	FontRenderer * renderer;
	HorizontalTextAlignment horizontalAlignment;
	VerticalTextAlignment verticalAlignment;

protected:
	void RepositionTextFromAlignment();

public:
	ZGILabelWidget(FontResource* font,ZGIVirtualWindow* owningWindow);
	~ZGILabelWidget();

	void SetText(const std::string& text);
	void SetFontSize(float pptSize);

	// widget overrides

	void SetHorizontalAlignment(HorizontalTextAlignment alignment);
	void SetVerticalAlignment(VerticalTextAlignment alignment);

	virtual void Render(const Matrix44& projection)override;

	virtual void SetSize(Vec2D size)override;
	virtual void SetPosition(Vec2D position)override;

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
};

