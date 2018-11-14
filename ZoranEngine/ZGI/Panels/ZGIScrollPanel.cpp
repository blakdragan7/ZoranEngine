#include "stdafx.h"
#include "ZGIScrollPanel.h"
#include <ZGI/Core/ZGIVirtualViewport.h>
#include <ZGI/Windows/ZGIVirtualWindow.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering\Renderers\GUIColoredRectRenderer.h>
#include <ZGI/Widgets/ZGILabelWidget.h>

#include <Core/CommomTypes.h>

#include <Core/PlatformMouseBase.h>

void ZGIScrollPanel::SizeAndPositionScrollBar()
{
	if (content == 0)return;

	Vec2D contentSize = content->GetSize();

	Vector2D sizeAlpha = (size / contentSize);

	bool canScrollVertical, canScrollHorizontal;

	canScrollVertical = sizeAlpha.h < 1.0f;
	canScrollHorizontal = sizeAlpha.w < 1.0f;

	scrollBarSize.w = size.w * 0.01f;
	scrollBarSize.h = sizeAlpha.h * size.h;

	scrollAlpha = (scrollOffset / contentSize);

	float adjustedSize = size.h - (scrollBarSize.h / 2.0f);

	float xPos = size.w - scrollBarSize.w;
	float yPos = ((1.0f-scrollAlpha.y) * adjustedSize);

	scrollAlpha /= (1.0f-sizeAlpha);

	if (scrollAlpha.y < 0 || scrollAlpha.y > 1.0f || canScrollVertical == false)
	{
		scrollOffset.y -= scrollDirection.y;
	}

	if (scrollAlpha.x < 0 || scrollAlpha.x > 1.0f || canScrollHorizontal == false)
	{
		scrollOffset.x -= scrollDirection.x;
	}

	Matrix44 scale = Matrix44::ScaleMatrix({ scrollBarSize.w / 2.0f, scrollBarSize.h / 2.0f, 1.0f });
	Matrix44 tr = Matrix44::TranslationMatrix({ xPos, yPos, 0.0f });

	scrollBarMVP = tr * scale;
}

ZGIScrollPanel::ZGIScrollPanel(ZGIVirtualWindow* owner) : isScrolling(false), content(0), ZGIPanel(owner)
{
	scrollBarRenderer = rEngine->CreateGUIColoredRectRenderer();
	scrollBarRenderer->SetLinearBlend();

	scrollBarRenderer->SetColorA({ 0.8f,0.8f,0.8f,1.0f });
	scrollBarRenderer->SetColorB({ 0.8f,0.8f,0.8f,1.0f });

	viewport = new ZGIVirtualViewport(this->position, this->size, owner->GetWindowSize());
	viewport->SetPosition({ 0,0 });

	size = Vector2D(100, 100);
}

ZGIScrollPanel::~ZGIScrollPanel()
{
	if (content)delete content;
	delete scrollBarRenderer;
}

bool ZGIScrollPanel::KeyEventSub(KeyEventType type, unsigned key)
{
	if (content)return content->KeyEvent(type, key);
	return false;
}

bool ZGIScrollPanel::CanAddWidget(ZGIWidget * widget) const
{
	return content == 0;
}

void ZGIScrollPanel::AddWidget(ZGIWidget * widget)
{
	if (auto label = dynamic_cast<ZGILabelWidget*>(widget))
	{
		label->SetShouldClipFont(false);
		//label->SetSizeToFont({ 0,1 });
	}

	if (content != widget)
	{
		content = widget;
	}
}

void ZGIScrollPanel::RemoveWidget(ZGIWidget * widget)
{
	if (content == widget)
	{
		content = 0;
	}
}

int ZGIScrollPanel::GetNumberOfWidgets() const
{
	return content == 0 ? 0 : 1;
}

int ZGIScrollPanel::GetMaxNumberOfWidgets() const
{
	return 1;
}

ZGIWidget * ZGIScrollPanel::WidgetForPosition(Vec2D pos)
{
	return content->HitTest(pos);
}

void ZGIScrollPanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: ancher stuff
	if (content)content->ContainerResized(newSize, oldSize);
}

void ZGIScrollPanel::SetSize(Vec2D size)
{
	viewport->SetSize(size);
	ZGIWidget::SetSize(size);
	if (content)content->SetSize(size);
}

void ZGIScrollPanel::SetPosition(Vec2D position)
{
	viewport->SetPosition(position);
	ZGIWidget::SetPosition(position);
	if (content)content->SetPosition(position);
}

void ZGIScrollPanel::Render(const Matrix44 & projection)
{
	if (isDirty)
	{
		SizeAndPositionScrollBar();
	}

	if (content)
	{
		Vec2D gOffset = owningWindow->GetGlobalOffsetCache();

		if (isScrolling)
		{
			scrollOffset += scrollDirection;

			SizeAndPositionScrollBar();
		}

		Matrix44 tr = Matrix44::TranslationMatrix({ -position.x + scrollOffset.x,-position.y + scrollOffset.y,0 });

		// clip everthing within this widget
		viewport->SetViewportActive(gOffset);
		auto proj = viewport->GetProjectionMatrix();
		content->Render(proj * tr);

		Matrix44 MVP = proj * scrollBarMVP;

		scrollBarRenderer->RenderObject(MVP);

		owningWindow->GetViewport()->SetViewportActive(gOffset);
	}

	ZGIWidget::Render(projection);
}

ZGIWidget * ZGIScrollPanel::HitTest(Vec2D pos)
{
	if (content)
	{
		ZGIWidget* ret = content->HitTest(pos);
		if (ret)return ret;
	}

	return ZGIWidget::HitTest(pos);
}

bool ZGIScrollPanel::MouseMove(const PlatformMouseBase * mouse)
{
	if (mouse->GetLeftMouseIsPressed())
	{
		isScrolling = true;

		scrollDirection = -mouse->GetDelta();
	}
	else
	{
		isScrolling = false;
	}

	return false;
}

bool ZGIScrollPanel::KeyEvent(KeyEventType type, unsigned key)
{
	if (type == KeyEventType_Key_Down)
	{
		isScrolling = true;
		
		switch (key)
		{
		case Key_Down_Arrow:
			scrollDirection.x = 0;
			scrollDirection.y = 1;
			break;
		case Key_Up_Arrow:
			scrollDirection.x = 0;
			scrollDirection.y = -1;
			break;
		case Key_Left_Arrow:
			scrollDirection.x = 1;
			scrollDirection.y = 0;
			break;
		case Key_Right_Arrow:
			scrollDirection.x = -1;
			scrollDirection.y = 0;
			break;
		}
	}
	if (type == KeyEventType_Key_Up)
	{
		isScrolling = false;
	}

	return true;
}
