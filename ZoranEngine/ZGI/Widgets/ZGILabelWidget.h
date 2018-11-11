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

	void SetShadowColor(Vec3D color);
	void SetShadowVector(Vec2D vector);
	void SetShadowSoftness(float softness);
	void SetShadowOpacity(float opacity);

	void SetFontThickness(float thickness);
	void SetFontBorder(float border);

	void SetText(const std::string& text);
	void SetFontSize(float pptSize);
	void SetFontColor(const Color& c);
	void SetBorderColor(const Color& c);

	void SetShouldWordWrap(bool wrap);
	void SetShouldClipFont(bool clip);

	void SetHorizontalAlignment(HorizontalTextAlignment alignment);
	void SetVerticalAlignment(VerticalTextAlignment alignment);

	// widget overrides
	virtual void Render(const Matrix44& projection)override;

	virtual void SetSize(Vec2D size)override;
	virtual void SetPosition(Vec2D position)override;

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
};

