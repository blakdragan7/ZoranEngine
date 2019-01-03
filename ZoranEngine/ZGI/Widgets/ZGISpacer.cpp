#include "stdafx.h"
#include "ZGISpacer.h"


ZGISpacer::ZGISpacer(ZGIVirtualWindow* window) : ZGIWidget(window)
{
}

ZGISpacer::~ZGISpacer()
{
}

void ZGISpacer::SetSize(Vec2D size)
{
	ZGIWidget::SetSize(size);
	bounds = size;
}

void ZGISpacer::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Ancher stuff
}
