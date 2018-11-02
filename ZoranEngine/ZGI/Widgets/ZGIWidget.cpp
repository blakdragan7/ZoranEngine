#include "stdafx.h"
#include "ZGIWidget.h"
#include <Core/PlatformMouseBase.h>
#include <ZGI/Windows/ZGIVirtualWindow.h>

void ZGIWidget::RecalculateModelCache()
{
	modelCache = translate * scale  * rotation;
}

ZGIWidget::ZGIWidget(ZGIVirtualWindow* owningWindow) : mouseHasEntered(false), owningWindow(owningWindow)
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
