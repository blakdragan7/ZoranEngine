#include "stdafx.h"
#include "ZGILabelWidget.h"

#include <Rendering/Primitives.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering/Renderers/FontRenderer.h>

void ZGILabelWidget::RepositionTextFromAlignment()
{
	float x = 0;
	float y = 0;

	float offset = renderer->GetPPTSize() * 0.7333f;

	switch (horizontalAlignment)
	{
	case HorizontalTextAlignment_Left:
		x = position.x;
		break;
	case HorizontalTextAlignment_Right:
		x = position.x + size.w - offset;
		break;
	case HorizontalTextAlignment_Center:
		x = position.x + (size.w / 2.0f) - offset;
		break;
	}

	switch (verticalAlignment)
	{
	case VerticalTextAlignment_Bottom:
		y = position.y;
		break;
	case VerticalTextAlignment_Top:
		y = position.y + size.h - offset;
		break;
	case VerticalTextAlignment_Center:
		y = position.y + (size.h / 2.0f) - offset;
		break;
	}

	renderer->SetRenderStart({ x, y });
}

ZGILabelWidget::ZGILabelWidget(FontResource* font, ZGIVirtualWindow* owningWindow) : verticalAlignment(VerticalTextAlignment_Top), horizontalAlignment(HorizontalTextAlignment_Left) , ZGIWidget(owningWindow)
{
	renderer = rEngine->CreateFontRenderer(font);

	size.x = 100; size.y = 100;
}

ZGILabelWidget::~ZGILabelWidget()
{
	delete renderer;
}

void ZGILabelWidget::SetShadowColor(Vec3D color)
{
	renderer->SetShadowColor(color);
}

void ZGILabelWidget::SetShadowVector(Vec2D vector)
{
	renderer->SetShadowVector(vector);
}

void ZGILabelWidget::SetShadowSoftness(float softness)
{
	renderer->SetShadowSoftness(softness);
}

void ZGILabelWidget::SetShadowOpacity(float opacity)
{
	renderer->SetShadowOpacity(opacity);
}

void ZGILabelWidget::SetFontThickness(float thickness)
{
	renderer->SetFontThickness(thickness);
}

void ZGILabelWidget::SetFontBorder(float border)
{
	renderer->SetFontBorder(border);
}

void ZGILabelWidget::SetText(const std::string & text)
{
	renderer->SetText(text);
}

void ZGILabelWidget::SetFontSize(float pptSize)
{
	renderer->SetPPTSize(pptSize);
}

void ZGILabelWidget::SetFontColor(const Color& c)
{
	renderer->SetFontColor(c);
}

void ZGILabelWidget::SetBorderColor(const Color & c)
{
	renderer->SetBorderColor(c);
}

void ZGILabelWidget::SetWordWrap(bool wrap)
{
	renderer->SetShouldWordWrap(wrap);
}

void ZGILabelWidget::SetClipFont(bool clip)
{
	renderer->SetShouldClip(clip);
}

void ZGILabelWidget::SetHorizontalAlignment(HorizontalTextAlignment alignment)
{
	if (horizontalAlignment != alignment)
	{
		horizontalAlignment = alignment;
		RepositionTextFromAlignment();
	}
}

void ZGILabelWidget::SetVerticalAlignment(VerticalTextAlignment alignment)
{
	if (verticalAlignment != alignment)
	{
		verticalAlignment = alignment;
		RepositionTextFromAlignment();
	}
}

void ZGILabelWidget::Render(const Matrix44 & projection)
{
	renderer->RenderObject(projection);

	ZGIWidget::Render(projection);
}

void ZGILabelWidget::SetSize(Vec2D size)
{
	renderer->SetBounds(size);
	this->size = size;

	RepositionTextFromAlignment();
	isDirty = true;
}

void ZGILabelWidget::SetPosition(Vec2D position)
{
	this->position = position;

	RepositionTextFromAlignment();
	isDirty = true;
}

void ZGILabelWidget::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: some ancher stuff
}
