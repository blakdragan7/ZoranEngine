#include "stdafx.h"
#include "ZGITabPanel.h"

#include <Utils/VectorAddons.hpp>

#include <ZGI/Panels/ZGISwitcherPanel.h>

ZGITabPanel::ZGITabPanel(ZGIVirtualWindow* owningWindow) : ZGIPanel(owningWindow)
{
	body = new ZGISwitcherPanel(owningWindow);
}

ZGITabPanel::~ZGITabPanel()
{
	delete body;
}

void ZGITabPanel::AddWidget(ZGIWidget * widget)
{
	body->AddWidget(widget);
}

void ZGITabPanel::RemoveWidget(ZGIWidget * widget)
{
	body->RemoveWidget(widget);
}
void ZGITabPanel::AnimateAllWidgets(float dt)
{
	body->AnimateAllWidgets(dt);
}

bool ZGITabPanel::ContainsWidget(ZGIWidget * widget) const
{
	return body->ContainsWidget(widget);
}

bool ZGITabPanel::CanAddWidget(ZGIWidget * widget) const
{
	return body->CanAddWidget(widget);
}

int ZGITabPanel::GetNumberOfWidgets() const
{
	return body->GetNumberOfWidgets();
}

int ZGITabPanel::GetMaxNumberOfWidgets() const
{
	return INT_MAX;
}

ZGIWidget * ZGITabPanel::WidgetForPosition(Vec2D pos)
{
	return body->WidgetForPosition(pos);
}

void ZGITabPanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	body->ContainerResized(newSize, oldSize);
}

void ZGITabPanel::Render(const Matrix44 & projection)
{
	body->Render(projection);

	ZGIPanel::Render(projection);
}
