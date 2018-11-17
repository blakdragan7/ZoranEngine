#include "stdafx.h"
#include "ZGIScrollBar.h"
#include <Rendering\Renderers\GUIColoredRectRenderer.h>
#include <Rendering/RenderEngineBase.h>


void ZGIScrollBar::SizeAndPositionScrollBar()
{
	if (isVertical)
	{
		Vector2D scrollBarSize(size.w * 1.0f ,sizeAlpha * size.h);
		float adjustedSize = size.h - scrollBarSize.h;

		float xPos = position.x + size.w - (scrollBarSize.w / 2.0f);
		float yPos = position.y + ((scrollBarSize.h / 2.0f) + (scrollAlpha * adjustedSize));

		Matrix44 scale = Matrix44::ScaleMatrix({ scrollBarSize.w / 2.0f, scrollBarSize.h / 2.0f, 1.0f });
		Matrix44 tr = Matrix44::TranslationMatrix({ xPos, yPos, 0.0f });
		scrollMVP = tr * scale;
	}
	else
	{
		Vector2D scrollBarSize(sizeAlpha * size.w, size.h * 1.0f);
		float adjustedSize = size.w - scrollBarSize.w;

		float xPos = position.x + ((scrollBarSize.w / 2.0f) + (scrollAlpha * adjustedSize));
		float yPos = position.y + (scrollBarSize.h / 2.0f);

		Matrix44 scale = Matrix44::ScaleMatrix({ scrollBarSize.w / 2.0f, scrollBarSize.h / 2.0f, 1.0f });
		Matrix44 tr = Matrix44::TranslationMatrix({ xPos, yPos, 0.0f });
		scrollMVP = tr * scale;
	}

}

ZGIScrollBar::ZGIScrollBar(bool isVertical, ZGIVirtualWindow* owningWindow) : isVertical(isVertical), scrollAlpha(0), ZGIWidget(owningWindow)
{
	renderer = rEngine->CreateGUIColoredRectRenderer();
	renderer->SetLinearBlend();

	renderer->SetColorA({ 0.8f,0.8f,0.8f,1.0f });
	renderer->SetColorB({ 0.8f,0.8f,0.8f,1.0f });
}


ZGIScrollBar::~ZGIScrollBar()
{
	delete renderer;
}

void ZGIScrollBar::Render(const Matrix44 & mvp)
{
	if (isDirty)
	{
		SizeAndPositionScrollBar();
	}

	renderer->RenderObject(mvp * scrollMVP);

	ZGIWidget::Render(mvp);
}

void ZGIScrollBar::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: ancher stuff
}

