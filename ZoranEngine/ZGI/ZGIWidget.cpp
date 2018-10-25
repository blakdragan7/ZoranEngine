#include "stdafx.h"
#include "ZGIWidget.h"


void ZGIWidget::RecalculateModelCache()
{
	modelCache = translate * rotation * scale;
}

ZGIWidget::ZGIWidget()
{
}


ZGIWidget::~ZGIWidget()
{
}

void ZGIWidget::SetSize(Vec2D size)
{
	scale.setScale({ size,1.0f });

	RecalculateModelCache();
}

void ZGIWidget::SetPosition(Vec2D position)
{
	translate.setTranslate(position);

	RecalculateModelCache();
}

void ZGIWidget::SetRotation(float rotation)
{
	this->rotation = Matrix44::RotationMatrix(rotation, { 0,0,1.0f });

	RecalculateModelCache();
}
