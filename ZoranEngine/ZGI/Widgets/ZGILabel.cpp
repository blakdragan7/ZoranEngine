#include "stdafx.h"
#include "ZGILabel.h"

#include <Resources/ResourceManager.h>

#include <Rendering/Primitives.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering/Renderers/FontRenderer.h>

#include <Core/PlatformMouseBase.h>
#include <Core/CommomTypes.h>

ZGILabel::ZGILabel(ZGIVirtualWindow * owningWindow) : setBoundsFromSize(true), fontNeedsUpdate(false), autoScaleFont(true), alignment(Alignment_Left | Alignment_Top), ZGIWidget(owningWindow)
{
	auto rm = RM;
	renderer = rEngine->CreateFontRenderer(rm->FontForPath("arial-msdf.zft"));
	size.x = 100; size.y = 100;
}

ZGILabel::ZGILabel(bool setBoundsFromSize, ZGIVirtualWindow * owningWindow) : setBoundsFromSize(setBoundsFromSize), fontNeedsUpdate(false), autoScaleFont(true), alignment(Alignment_Left | Alignment_Top), ZGIWidget(owningWindow)
{
	auto rm = RM;
	renderer = rEngine->CreateFontRenderer(rm->FontForPath("arial-msdf.zft"));
	size.x = 100; size.y = 100;
}

ZGILabel::ZGILabel(FontResource font, ZGIVirtualWindow* owningWindow) : setBoundsFromSize(true), fontNeedsUpdate(false), autoScaleFont(true), alignment(Alignment_Left | Alignment_Top), ZGIWidget(owningWindow)
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

void ZGILabel::SetTextBounds(Vec2D bounds)
{
	renderer->SetBounds(position, bounds);
	textRenderBounds = bounds;
	fontNeedsUpdate = true;
}

inline void ZGILabel::SetAlignment(unsigned alignment)
{
	if (this->alignment != alignment)
	{
		this->alignment = alignment;
		renderer->SetAlignment(alignment);
	}
}

void ZGILabel::Render(const Matrix44 & projection)
{
	if (isDirty || fontNeedsUpdate)
	{
		renderer->SetBounds(position, size);
		fontNeedsUpdate = false;
	}

	ZGIWidget::Render(projection);

	renderer->RenderObject(projection);
}

void ZGILabel::SetSize(Vec2D size)
{
	if (autoScaleFont)renderer->SetPPTSize(size.h * 0.9f);
	if (setBoundsFromSize)
	{
		renderer->SetBounds(position, size);
		textRenderBounds = size;
	}
	ZGIWidget::SetSize(size);
}

void ZGILabel::SetPosition(Vec2D position)
{
	this->position = position;

	ZGIWidget::SetPosition(position);
}

Vector2D ZGILabel::GetBounds() const
{
	return Vector2D((float)renderer->GetMaxLineSize() * renderer->GetPPTSize(), (float)renderer->GetLineCount() * renderer->GetPPTSize());
}

void ZGILabel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: some ancher stuff
}
