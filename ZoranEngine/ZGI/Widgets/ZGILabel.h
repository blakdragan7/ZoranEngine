#pragma once
#include<ZGI\Widgets\ZGIWidget.h>

enum Alignment;
class FontRenderer;
class FontResource;
class ZoranEngine_EXPORT ZGILabel : public ZGIWidget
{
private:
	unsigned alignment;
	bool autoScaleFont;
	bool fontNeedsUpdate;
	bool setBoundsFromSize;

protected:
	FontRenderer * renderer;

	Vector2D textRenderBounds;

protected:
	void RepositionTextFromAlignment();

public:
	ZGILabel(ZGIVirtualWindow* owningWindow);
	ZGILabel(bool setBoundsFromSize, ZGIVirtualWindow* owningWindow);
	ZGILabel(FontResource* font,ZGIVirtualWindow* owningWindow);
	~ZGILabel();

	inline void SetAutoScaleFont(bool autoScale) { autoScaleFont = autoScale; }

	inline void SetBoundsFromSize(bool setBounds) { setBoundsFromSize = setBounds; }

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

	void SetTextBounds(Vec2D bounds);

	inline void SetAlignment(unsigned alignment) { this->alignment = alignment; isDirty = true; }

	// widget overrides
	virtual void Render(const Matrix44& projection)override;

	virtual void SetSize(Vec2D size)override;
	virtual void SetPosition(Vec2D position)override;

	virtual Vector2D GetBounds()const override;

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
	
	virtual bool DoesContainText()const override { return true; }
	virtual const char* GetClassString()const override { return "ZGILabel"; }
};

