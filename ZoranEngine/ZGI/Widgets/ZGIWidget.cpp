#include "stdafx.h"
#include "ZGIWidget.h"
#include <Core/PlatformMouseBase.h>
#include <ZGI/Windows/ZGIVirtualWindow.h>
#include <ZGI/Core/ZGIBrush.h>
#include <Rendering/TextureBase.h>

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

void ZGIWidget::Print(unsigned tabs)const
{
	for (unsigned i = 0; i < tabs; i++)
	{
		Log(LogLevel_Debug, "\t");
	}

	Log(LogLevel_Debug, " %s: Size {%f,%f} Position {%f,%f}\n", GetClassString(), size.w, size.h, position.x, position.y);
}

void ZGIWidget::RenderWithPositionAndSize(Vec2D position, Vec2D size, const Matrix44& projection)
{
	Vector2D oldPosition = this->position;
	Vector2D oldSize = this->size;

	SetSize(size);
	SetPosition(position);

	Render(projection);

	SetSize(oldSize);
	SetPosition(oldPosition);
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

void ZGIWidget::Animate(float dt)
{
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

void ZGIWidget::SetRotation(float rotation)
{
	this->rotation = Matrix44::RotationMatrix(rotation, { 0,0,1.0f });

	isDirty = true;
}

ZGIWidget* ZGIWidget::HitTest(Vec2D pos)
{
	return (position.x <= pos.x && (position.x + size.w) >= pos.x && position.y <= pos.y && (position.y+size.h) >= pos.y) ? this : 0;
}

Vector2D ZGIWidget::GetBounds() const
{
	const TextureBase* image = widgetBrush->GetBackgroundImage();
	if (image)return image->GetSize();
	else return bounds;
}

bool ZGIWidget::MouseMove(const PlatformMouseBase& m)
{
	if (m.GetAnyButtonIsPressed())
	{
		if (mouseHasEntered)
		{
			MouseLeft(m);
			mouseHasEntered = false;
		}
	}

	Vector2D translatedPos = owningWindow->ConvertAbsoluteToVirtual(m.GetPosition());

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
