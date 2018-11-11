#include "stdafx.h"
#include "ZGIScrollPanel.h"
#include <ZGI/Core/ZGIVirtualViewport.h>
#include <ZGI/Windows/ZGIVirtualWindow.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering/Renderers/TriangleStripRenderer.h>

#include <ZGI/Widgets/ZGILabelWidget.h>


#include <Core/CommomTypes.h>

ZGIScrollPanel::ZGIScrollPanel(ZGIVirtualWindow* owner) : isScrolling(false), content(0), ZGIPanel(owner)
{
	renderer = rEngine->CreateTriangleStripRenderer();
	viewport = new ZGIVirtualViewport(this->position, this->size, owner->GetWindowSize());
	viewport->SetPosition({ 0,0 });
}

ZGIScrollPanel::~ZGIScrollPanel()
{
	if (content)delete content;
	delete renderer;
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
	}

	if (content != widget)
	{
		SetSize(widget->GetSize());
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
	ZGIWidget::SetPosition(-position);
	if (content)content->SetPosition(position);
}

void ZGIScrollPanel::Render(const Matrix44 & projection)
{
	Vec2D gOffset = owningWindow->GetGlobalOffsetCache();

	Matrix44 tr(false);

	if (isScrolling)
		scrollOffset += scrollDirection;

	tr = Matrix44::TranslationMatrix({ position.x + scrollOffset.x,position.y + scrollOffset.y,0 });

	// clip everthing within this widget
	viewport->SetViewportActive(gOffset);
	auto proj = viewport->GetProjectionMatrix();
	if (content)content->Render(proj * tr);

	owningWindow->GetViewport()->SetViewportActive(gOffset);
	renderer->RenderObject(projection);

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

bool ZGIScrollPanel::KeyEvent(KeyEventType type, unsigned key)
{
	if (type == KeyEventType_Key_Down)
	{
		isScrolling = true;
		
		switch (key)
		{
		case Key_Down_Arrow:
			scrollDirection.x = 0;
			scrollDirection.y = -1;
			break;
		case Key_Up_Arrow:
			scrollDirection.x = 0;
			scrollDirection.y = 1;
			break;
		case Key_Left_Arrow:
			scrollDirection.x = -1;
			scrollDirection.y = 0;
			break;
		case Key_Right_Arrow:
			scrollDirection.x = 1;
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
