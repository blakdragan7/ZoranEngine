#include "stdafx.h"
#include "ZGILabel.h"

#include <Rendering/Primitives.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering/Renderers/FontRenderer.h>

#include <Core/PlatformMouseBase.h>

void ZGILabel::RepositionTextFromAlignment()
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

ZGILabel::ZGILabel(FontResource* font, ZGIVirtualWindow* owningWindow) : verticalAlignment(VerticalTextAlignment_Top), horizontalAlignment(HorizontalTextAlignment_Left) , ZGIWidget(owningWindow)
{
	renderer = rEngine->CreateFontRenderer(font);

	size.x = 100; size.y = 100;
}

ZGILabel::~ZGILabel()
{
	delete renderer;
}

void ZGILabel::SetShadowColor(Vec3D color)
{
	renderer->SetShadowColor(color);
}

void ZGILabel::SetShadowVector(Vec2D vector)
{
	renderer->SetShadowVector(vector);
}

void ZGILabel::SetShadowSoftness(float softness)
{
	renderer->SetShadowSoftness(softness);
}

void ZGILabel::SetShadowOpacity(float opacity)
{
	renderer->SetShadowOpacity(opacity);
}

void ZGILabel::SetFontThickness(float thickness)
{
	renderer->SetFontThickness(thickness);
}

void ZGILabel::SetFontBorder(float border)
{
	renderer->SetFontBorder(border);
}

void ZGILabel::SetText(const std::string & text)
{
	renderer->SetText(text);
}

void ZGILabel::SetFontSize(float pptSize)
{
	renderer->SetPPTSize(pptSize);
}

void ZGILabel::SetFontColor(const Color& c)
{
	renderer->SetFontColor(c);
}

void ZGILabel::SetBorderColor(const Color & c)
{
	renderer->SetBorderColor(c);
}

void ZGILabel::SetShouldWordWrap(bool wrap)
{
	renderer->SetShouldWordWrap(wrap);
}

void ZGILabel::SetShouldClipFont(bool clip)
{
	renderer->SetShouldClip(clip);
}

void ZGILabel::SetHorizontalAlignment(HorizontalTextAlignment alignment)
{
	if (horizontalAlignment != alignment)
	{
		horizontalAlignment = alignment;
		isDirty = true;
	}
}

void ZGILabel::SetVerticalAlignment(VerticalTextAlignment alignment)
{
	if (verticalAlignment != alignment)
	{
		verticalAlignment = alignment;
		isDirty = true;
	}
}

void ZGILabel::Render(const Matrix44 & projection)
{
	if (isDirty)
	{
		RepositionTextFromAlignment();
	}

	renderer->RenderObject(projection);

	ZGIWidget::Render(projection);
}

void ZGILabel::SetSize(Vec2D size)
{
	SetBounds(size);
	ZGIWidget::SetSize(size);
}

void ZGILabel::SetBounds(Vec2D bounds)
{
	renderer->SetBounds(bounds);
	this->bounds = bounds;

	isDirty = true;
}

void ZGILabel::SetPosition(Vec2D position)
{
	this->position = position;

	RepositionTextFromAlignment();
	isDirty = true;

	ZGIWidget::SetPosition(position);
}

void ZGILabel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: some ancher stuff
}

Vector2D ZGILabel::GetSize()
{
	if (isDirty)
		renderer->UpdateRender();

	return renderer->GetTotalSize();
}
