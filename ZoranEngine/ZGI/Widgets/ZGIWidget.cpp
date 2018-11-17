#include "stdafx.h"
#include "ZGIWidget.h"
#include <Core/PlatformMouseBase.h>
#include <ZGI/Windows/ZGIVirtualWindow.h>
#include <Rendering/Renderers/LineLoopRenderer.h>
#include <Rendering/RenderEngineBase.h>

void ZGIWidget::UpdateDebugRender()
{
	renderer->BeginAddingSegments();

	Vector2D padding(0, 0);
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

void ZGIWidget::RecalculateModelCache()
{
	modelCache = translate * scale  * rotation;
}

ZGIWidget::ZGIWidget(ZGIVirtualWindow* owningWindow) : drawDebugView(false), drawEditorView(false), isDirty(false), mouseHasEntered(false), owningWindow(owningWindow)
{
	modelCache.makeIdentity();
	translate.makeIdentity();
	rotation.makeIdentity();
	scale.makeIdentity();

	renderer = rEngine->CreateLineLoopRenderer();
}

ZGIWidget::~ZGIWidget()
{
	delete renderer;
}

void ZGIWidget::Render(const Matrix44 & projection)
{
	if (isDirty)
	{
		isDirty = false;
		UpdateDebugRender();
	}

	if (drawDebugView || drawEditorView)
		renderer->RenderObject(projection);
}

void ZGIWidget::SetSize(Vec2D size)
{
	Vector2D newSize = size / 2.0f;
	newSize.y = -newSize.y;
	scale.setScale({ newSize,1.0f });
	this->size = size;
	translate.setTranslate(position + (size / 2.0f));

	RecalculateModelCache();

	isDirty = true;
}

void ZGIWidget::SetPosition(Vec2D position)
{
	translate.setTranslate(position + (size / 2.0f));
	this->position = position;
	RecalculateModelCache();

	isDirty = true;
}

void ZGIWidget::SetBounds(Vec2D bounds)
{
	this->bounds = bounds;
}

void ZGIWidget::SetRotation(float rotation)
{
	this->rotation = Matrix44::RotationMatrix(rotation, { 0,0,1.0f });

	RecalculateModelCache();

	isDirty = true;
}

ZGIWidget* ZGIWidget::HitTest(Vec2D pos)
{
	return (position.x <= pos.x && (position.x + size.w) >= pos.x && position.y <= pos.y && (position.y+size.h) >= pos.y) ? this : 0;
}

bool ZGIWidget::MouseMove(const PlatformMouseBase* m)
{
	if (m->GetAnyButtonIsPressed())
	{
		if (mouseHasEntered)
		{
			MouseLeft(m);
			mouseHasEntered = false;
		}
	}

	Vector2D translatedPos = owningWindow->ConvertAbsoluteToVirtual(m->GetPosition());

	if (mouseHasEntered == false)
	{
		if (HitTest(translatedPos) == this)
		{
			mouseHasEntered = true;
			MouseEnterd(m);
		}
	}
	if (mouseHasEntered == true)
	{
		if (HitTest(translatedPos) == 0)
		{
			mouseHasEntered = false;
			MouseLeft(m);
		}
	}
	
	//TODO: optimize this better so that it doesn't have to give this event to every UI element

	return false;
}
