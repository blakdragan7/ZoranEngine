#include "stdafx.h"
#include "ZGITreePanel.h"


ZGITreePanel::ZGITreePanel(ZGIVirtualWindow* owningWindow) : ZGIPanel(owningWindow)
{
}


ZGITreePanel::~ZGITreePanel()
{
}

bool ZGITreePanel::KeyEventSub(KeyEventType type, unsigned key)
{
	return false;
}

bool ZGITreePanel::ContainsWidget(ZGIWidget * widget) const
{
	return false;
}

bool ZGITreePanel::CanAddWidget(ZGIWidget * widget) const
{
	return false;
}

int ZGITreePanel::GetNumberOfWidgets() const
{
	return 0;
}

int ZGITreePanel::GetMaxNumberOfWidgets() const
{
	return 0;
}

ZGIWidget * ZGITreePanel::WidgetForPosition(Vec2D pos)
{
	return nullptr;
}

void ZGITreePanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
}

void ZGITreePanel::Render(const Matrix44 & projection)
{
}
