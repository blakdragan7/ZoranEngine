#include "stdafx.h"
#include "ZGIWidget.h"
#include <Core/PlatformMouseBase.h>
#include <ZGI/Windows/ZGIVirtualWindow.h>
#include <ZGI/Core/ZGIBrush.h>

void ZGIWidget::RecalculateModelCache()
{
	modelCache = translate * scale  * rotation;
}

ZGIWidget::ZGIWidget(ZGIVirtualWindow* owningWindow) : shouldDrawBrush(false), isDirty(false), mouseHasEntered(false), owningWindow(owningWindow)
{
	modelCache.makeIdentity();
	translate.makeIdentity();
	rotation.makeIdentity();
	scale.makeIdentity();

	widgetBrush = new ZGIBrush;
}

ZGIWidget::~ZGIWidget()
{
	delete widgetBrush;
}

void ZGIWidget::Render(const Matrix44 & projection)
{
	if (isDirty)
	{
		isDirty = false;
		RecalculateModelCache();
	}
	
	if(shouldDrawBrush)
		widgetBrush->RenderBrush(projection * modelCache);
}

void ZGIWidget::SetSize(Vec2D size)
{
	Vector2D newSize = size / 2.0f;
	newSize.y = -newSize.y;
	scale.setScale({ newSize,1.0f });
	this->size = size;
	translate.setTranslate(position + (size / 2.0f));

	isDirty = true;
}

void ZGIWidget::SetPosition(Vec2D position)
{
	translate.setTranslate(position + (size / 2.0f));
	this->position = position;

	isDirty = true;
}

void ZGIWidget::SetBounds(Vec2D bounds)
{
	this->bounds = bounds;
}

void ZGIWidget::SetRotation(float rotation)
{
	this->rotation = Matrix44::RotationMatrix(rotation, { 0,0,1.0f });

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
