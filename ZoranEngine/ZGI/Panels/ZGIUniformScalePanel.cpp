#include "stdafx.h"
#include "ZGIUniformScalePanel.h"

void ZGIUniformScalePanel::RepositionContent()
{
	/* shrink content to fit in aspect */
	if (content)
	{
		Vector2D contentSize;

		Vector2D ratio = size / contentStartingSize;

		if (ratio.h > ratio.w)
			contentSize = Vector2D(contentStartingSize.w * ratio.w, contentStartingSize.h * ratio.w);
		else
			contentSize = Vector2D(contentStartingSize.w * ratio.h, contentStartingSize.h * ratio.h);

		content->SetSize(contentSize);
	}

	if (adjustPosition)
	{
		Vector2D contentHalfSize = content->GetSize() / 2.0f;
		Vector2D halfSize = size / 2.0f;
		float x = (position.x + halfSize.w) - contentHalfSize.w;
		float y = (position.y + halfSize.h) - contentHalfSize.h;

		content->SetPosition({ x,y });
	}
}

ZGIUniformScalePanel::ZGIUniformScalePanel(ZGIVirtualWindow* owningWindow) : content(0), adjustPosition(true), ZGIPanel(owningWindow)
{
}

ZGIUniformScalePanel::~ZGIUniformScalePanel()
{
	if (content)delete content;
}

void ZGIUniformScalePanel::AnimateAllWidgets(float dt)
{
	if(content)content->Animate(dt);
}

bool ZGIUniformScalePanel::ContainsWidget(ZGIWidget * widget) const
{
	return content == widget;
}

bool ZGIUniformScalePanel::CanAddWidget(ZGIWidget * widget)const
{
	return content == 0;
}

void ZGIUniformScalePanel::AddWidget(ZGIWidget * widget)
{
	if (content != 0)
	{
		Log(LogLevel_Error, "Adding New Widget To Unfirom scale panel when widget already exists !");
		assert(false);
	}

	if (widget->DoesContainText())
	{
		adjustPosition = false;
	}
	else
	{
		adjustPosition = true;
	}

	contentStartingSize = widget->GetBounds();

	content = widget;

	RepositionContent();
}

void ZGIUniformScalePanel::RemoveWidget(ZGIWidget * widget)
{
	if (content == widget)
	{
		delete content;
		content = 0;
	}
}

int ZGIUniformScalePanel::GetNumberOfWidgets() const
{
	return content == 0 ? 0 : 1;
}

int ZGIUniformScalePanel::GetMaxNumberOfWidgets() const
{
	return 1;
}

ZGIWidget * ZGIUniformScalePanel::WidgetForPosition(Vec2D pos)
{
	return content->HitTest(pos) ? content : 0;
}

void ZGIUniformScalePanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: some kind of anchering system
	SetSize(newSize);
}

void ZGIUniformScalePanel::SetSize(Vec2D size)
{
	ZGIWidget::SetSize(size);
	if(content)RepositionContent();
}

void ZGIUniformScalePanel::SetPosition(Vec2D position)
{
	/* always center content */

	ZGIWidget::SetPosition(position);
}

void ZGIUniformScalePanel::Render(const Matrix44 & projection)
{
	if (content)
	{
		if (isDirty)
		{
			RepositionContent();
		}
		content->Render(projection);
	}
	ZGIWidget::Render(projection);
}

void ZGIUniformScalePanel::Print(unsigned tabs) const
{
	ZGIWidget::Print(tabs);

	content->Print(tabs + 1);
}
