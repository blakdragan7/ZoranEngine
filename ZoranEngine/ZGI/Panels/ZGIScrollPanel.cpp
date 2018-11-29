#include "stdafx.h"
#include "ZGIScrollPanel.h"
#include <ZGI/Core/ZGIVirtualViewport.h>
#include <ZGI/Windows/ZGIVirtualWindow.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering\Renderers\GUIColoredRectRenderer.h>
#include <ZGI/Widgets/ZGILabel.h>
#include <ZGI/Widgets/ZGIScrollBar.h>
#include <Core/CommomTypes.h>

#include <Core/PlatformMouseBase.h>

void ZGIScrollPanel::SizeAndPositionScrollBar()
{
	if (content == 0)return;

	Vec2D contentSize = content->GetSize();

	Vector2D sizeAlpha = (size / contentSize);

	canScrollVertical = sizeAlpha.h < 1.0f;
	canScrollHorizontal = sizeAlpha.w < 1.0f;

	if (canScrollHorizontal == false)
	{
		scrollOffset.x = 0;
	}

	if (canScrollVertical == false)
	{
		scrollOffset.y = 0;
	}

	Vector2D scrollAlpha = (scrollOffset / (contentSize - size));

	if (scrollAlpha.y < 0 || scrollAlpha.y > 1.0f)
	{
		scrollOffset.y -= scrollDirection.y;
	}

	if (scrollAlpha.x < 0 || scrollAlpha.x > 1.0f)
	{
		scrollOffset.x -= scrollDirection.x;
	}

	if (canScrollHorizontal)
	{
		hScrollBar->SetScrollBarSizeAlpha(sizeAlpha.w);
		hScrollBar->SetScrollPositionAlpha(scrollAlpha.x);
	}
	if (canScrollVertical)
	{
		vScrollBar->SetScrollBarSizeAlpha(sizeAlpha.h);
		vScrollBar->SetScrollPositionAlpha(1.0f - scrollAlpha.y);
	}
}

ZGIScrollPanel::ZGIScrollPanel(ZGIVirtualWindow* owner) : isScrolling(false), content(0), ZGIPanel(owner)
{
	hScrollBar = new ZGIScrollBar(false, owner);
	vScrollBar = new ZGIScrollBar(true, owner);

	viewport = new ZGIVirtualViewport(this->position, this->size, owner->GetWindowSize());
	viewport->SetPosition({ 0,0 });

	size = Vector2D(100, 100);

	hScrollBar->SetSize(size * Vector2D(1.0f,0.01f));
	vScrollBar->SetSize(size * Vector2D(0.001f,1.0f));
}

ZGIScrollPanel::~ZGIScrollPanel()
{
	if (content)delete content;
	delete hScrollBar;
	delete vScrollBar;
}

bool ZGIScrollPanel::KeyEventSub(KeyEventType type, unsigned key)
{
	if (content)return content->KeyEvent(type, key);
	return false;
}

bool ZGIScrollPanel::ContainsWidget(ZGIWidget * widget) const
{
	return content == widget;
}

bool ZGIScrollPanel::CanAddWidget(ZGIWidget * widget) const
{
	return content == 0;
}

void ZGIScrollPanel::AddWidget(ZGIWidget * widget)
{
	if (auto label = dynamic_cast<ZGILabel*>(widget))
	{
		label->SetShouldClipFont(false);
		label->SetPosition({ 0,0 });
	}

	if (content != widget)
	{
		if (content)delete content;
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

	Vector2D sSize(size.w*0.01f,size.h);

	vScrollBar->SetSize(sSize);

	vScrollBar->SetPosition({size.w - sSize.w,0.0f});

	sSize.w = size.w;
	sSize.h = size.h * 0.01f;

	hScrollBar->SetSize(sSize);

	if (content)content->SetBounds(size);
}

void ZGIScrollPanel::SetPosition(Vec2D position)
{
	viewport->SetPosition(position);
	ZGIWidget::SetPosition(position);
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
		Matrix44 tr(false);

		if (isScrolling)
		{
			scrollOffset += scrollDirection;

			SizeAndPositionScrollBar();
		}

		Vec2D contentPosition = content->GetPosition();

		if (content->DoesContainText())
		{
			tr = Matrix44::TranslationMatrix({ -scrollOffset.x,scrollOffset.y + size.h,0 });
		}
		else
		{
			Vec2D contentSize = content->GetSize();
			tr = Matrix44::TranslationMatrix({ -scrollOffset.x - contentPosition.x,scrollOffset.y - contentPosition.y - (contentSize.h - size.h),0 });
		}

		// clip everthing within this widget
		viewport->SetViewportActive(gOffset);
		auto proj = viewport->GetProjectionMatrix();
		content->Render(proj * tr);

		if(canScrollHorizontal)
			hScrollBar->Render(proj);
		if(canScrollVertical)
			vScrollBar->Render(proj);

		owningWindow->GetViewport()->SetViewportActive(gOffset);
	}

	ZGIWidget::Render(projection);
}

ZGIWidget * ZGIScrollPanel::HitTest(Vec2D pos)
{
	// TODO: uncomment this and hit test scrollbars instead
	/*if (content)
	{
		ZGIWidget* ret = content->HitTest(pos);
		if (ret)return ret;
	}*/

	return ZGIWidget::HitTest(pos);
}

bool ZGIScrollPanel::MouseMove(const PlatformMouseBase * mouse)
{
	if (mouse->GetLeftMouseIsPressed())
	{
		isScrolling = true;

		Vec2D delta = mouse->GetDelta();

		scrollDirection.x = -delta.x;
		scrollDirection.y = -delta.y;
	}
	else
	{
		isScrolling = false;
	}

	return ZGIWidget::MouseMove(mouse);
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
