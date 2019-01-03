#include "stdafx.h"
#include "ZGILabel.h"

#include <Core/Resources/ResourceManager.h>

#include <Rendering/Primitives.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering/Renderers/FontRenderer.h>

#include <Core/PlatformMouseBase.h>
#include <Core/CommomTypes.h>

void ZGILabel::RepositionTextFromAlignment()
{
	// TODO: make this render correctly, center text instead of centering renderstart etc .. 

	float x = 0;
	float y = 0;

	float offset = renderer->GetPPTSize() * 0.7333f;

	bool hasSetX, hasSetY;
	hasSetX = hasSetY = false;

	if (alignment & Alignment_Bottom)
	{
		y = position.y;
		hasSetY = true;
	}
	if (alignment & Alignment_Left)
	{
		x = position.x;
		hasSetX = true;
	}
	if (alignment & Alignment_Right)
	{
		if (hasSetX)Log(LogLevel_Warning, "Only one of Alignment_Right or Alignment_Left should be set for Alignment but Both are set!!");
		x = position.x + textRenderBounds.w - offset;
		hasSetX = true;
	}
	if (alignment & Alignment_Top)
	{
		if (hasSetY)Log(LogLevel_Warning, "Only one of Alignment_Top or Alignment_Bottom should be set for Alignment but Both are set!!");
		y = position.y + textRenderBounds.h - offset;
		hasSetY = true;
	}
	if (alignment & Alignment_Center)
	{
		if(hasSetX == false)x = position.x + (textRenderBounds.w / 2.0f) - (renderer->GetPPTSize() * 0.5f);
		if(hasSetY == false)y = position.y + (textRenderBounds.h / 2.0f) - (renderer->GetPPTSize() * 0.5f);
	}

	renderer->SetRenderStart({ x, y });
}

ZGILabel::ZGILabel(ZGIVirtualWindow * owningWindow) : setBoundsFromSize(false), fontNeedsUpdate(false), autoScaleFont(true), alignment(Alignment_Left | Alignment_Top), ZGIWidget(owningWindow)
{
	auto rm = RM;
	auto font = rm->FontForZFT("arial-msdf.zft");
	renderer = rEngine->CreateFontRenderer(font);
	size.x = 100; size.y = 100;
}

ZGILabel::ZGILabel(FontResource* font, ZGIVirtualWindow* owningWindow) : setBoundsFromSize(false), fontNeedsUpdate(false), autoScaleFont(true), alignment(Alignment_Left | Alignment_Top), ZGIWidget(owningWindow)
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
	renderer->SetBounds(bounds);
	textRenderBounds = bounds;
	fontNeedsUpdate = true;
}

void ZGILabel::Render(const Matrix44 & projection)
{
	if (isDirty || fontNeedsUpdate)
	{
		RepositionTextFromAlignment();
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
		renderer->SetBounds(size);
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
