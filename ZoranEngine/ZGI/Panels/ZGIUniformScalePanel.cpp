#include "stdafx.h"
#include "ZGIUniformScalePanel.h"
#include <Rendering/Renderers/LineLoopRenderer.h>
#include <Rendering/RenderEngineBase.h>

void ZGIUniformScalePanel::UpdateRender()
{
	renderer->BeginAddingSegments();

	Vector2D padding(5,5);
	Vector2D nSize = size - (padding * 2);
	Vector2D nPosition = position + padding;

	LineSegment currentSegment;

	currentSegment.vertecies[0] = nPosition;
	currentSegment.vertecies[1] = nPosition + Vector2D(0, nSize.h);

	renderer->AddSegment(currentSegment);

	currentSegment.vertecies[0] = nPosition + Vector2D(nSize.w, nSize.h);
	currentSegment.vertecies[1] = nPosition + Vector2D(nSize.w, 0);

	renderer->AddSegment(currentSegment);

	renderer->EndAddingSegments();
}

void ZGIUniformScalePanel::RepositionContent()
{
	Vector2D contentHalfSize = content->GetSize() / 2.0f;
	Vector2D halfSize = size / 2.0f;
	float x = (position.x + halfSize.w) - contentHalfSize.w;
	float y = (position.y + halfSize.h) - contentHalfSize.h;

	content->SetPosition({ x,y });
}

ZGIUniformScalePanel::ZGIUniformScalePanel() : content(0), isDirty(false)
{
	renderer = rEngine->CreateLineLoopRenderer();
}

ZGIUniformScalePanel::~ZGIUniformScalePanel()
{
	if (content)delete content;
	delete renderer;
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

	// force copy
	contentStartingSize = Vector2D(widget->GetSize());

	content = widget;
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
	/* shrink content to fit in aspect */
	if (content)
	{
		Vector2D contentSize;

		Vector2D ratio = size / contentStartingSize;

		if (ratio.h > ratio.w)
			contentSize = Vector2D(contentStartingSize.w * ratio.w, contentStartingSize.h * ratio.w);
		if (ratio.w > ratio.h)
			contentSize = Vector2D(contentStartingSize.w * ratio.h, contentStartingSize.h * ratio.h);

		content->SetSize(contentSize);
	}

	isDirty = true;
	ZGIWidget::SetSize(size);
	RepositionContent();
}

void ZGIUniformScalePanel::SetPosition(Vec2D position)
{
	/* always center content */

	if (content)
	{
		RepositionContent();
	}

	isDirty = true;
	ZGIWidget::SetPosition(position);
}

void ZGIUniformScalePanel::Render(const Matrix44 & projection)
{
	if (isDirty)
	{
		isDirty = false;
		UpdateRender();
	}

	if(content)
		content->Render(projection);

	if(drawDebugView || drawEditorView)
		renderer->RenderObject(projection);
}
