#include "stdafx.h"
#include "ZGIWidget.h"
#include <Core/PlatformMouseBase.h>

void ZGIWidget::RecalculateModelCache()
{
	modelCache = translate * scale  * rotation;
}

ZGIWidget::ZGIWidget() : mouseHasEntered(false)
{
	modelCache.makeIdentity();
	translate.makeIdentity();
	rotation.makeIdentity();
	scale.makeIdentity();
}

ZGIWidget::~ZGIWidget()
{
}

void ZGIWidget::SetSize(Vec2D size)
{
	Vector2D newSize = size / 2.0f;
	newSize.y = -newSize.y;
	scale.setScale({ newSize,1.0f });
	this->size = size;
	translate.setTranslate(position + (size / 2.0f));

	RecalculateModelCache();
}

void ZGIWidget::SetPosition(Vec2D position)
{
	translate.setTranslate(position + (size / 2.0f));
	this->position = position;
	RecalculateModelCache();
}

void ZGIWidget::SetRotation(float rotation)
{
	this->rotation = Matrix44::RotationMatrix(rotation, { 0,0,1.0f });

	RecalculateModelCache();
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

	if (mouseHasEntered == false)
	{
		if (HitTest(m->GetWindowSpacePosition()) == this)
		{
			mouseHasEntered = true;
			MouseEnterd(m);
		}
	}
	if (mouseHasEntered == true)
	{
		if (HitTest(m->GetWindowSpacePosition()) == 0)
		{
			mouseHasEntered = false;
			MouseLeft(m);
		}
	}
	
	//TODO: optimize this better so that it doesn't have to give this event to every UI element

	return false;
}
