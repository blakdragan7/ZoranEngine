#include "stdafx.h"
#include "ZGIWidget.h"


void ZGIWidget::RecalculateModelCache()
{
	modelCache = translate * scale  * rotation;
}

ZGIWidget::ZGIWidget()
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

bool ZGIWidget::HitTest(Vec2D pos)
{
	return (position.x <= pos.x && (position.x + size.w) >= pos.x && position.y <= pos.y && (position.y+size.h) >= pos.y);
}
