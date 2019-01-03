#include "stdafx.h"
#include "ZGIWrapBoxPanel.h"


void ZGIWrapBoxPanel::UpdateWidgetPositions()
{

}

ZGIWrapBoxPanel::ZGIWrapBoxPanel(ZGIVirtualWindow* owningWindow) : needsUpdate(false), ZGIPanel(owningWindow)
{
	widgets = new std::vector<ZGIWidget*>;
}

ZGIWrapBoxPanel::~ZGIWrapBoxPanel()
{
	delete widgets;
}

bool ZGIWrapBoxPanel::ContainsWidget(ZGIWidget * widget) const
{
	return std::find(widgets->begin(), widgets->end(), widget) != widgets->end();
}

bool ZGIWrapBoxPanel::CanAddWidget(ZGIWidget * widget) const
{
	return ContainsWidget(widget) == false;
}

int ZGIWrapBoxPanel::GetNumberOfWidgets() const
{
	return (int)widgets->size();
}

int ZGIWrapBoxPanel::GetMaxNumberOfWidgets() const
{
	return INT_MAX;
}

ZGIWidget * ZGIWrapBoxPanel::WidgetForPosition(Vec2D pos)
{
	for (auto& w : *widgets)
	{
		if (auto h = w->HitTest(pos))return h;
	}

	return nullptr;
}

void ZGIWrapBoxPanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: ancher stuff
}

void ZGIWrapBoxPanel::Render(const Matrix44 & projection)
{
	if (isDirty || needsUpdate)
	{
		UpdateWidgetPositions();
		needsUpdate = false;
	}

	ZGIPanel::Render(projection);

	for (auto& w : *widgets)
	{
		w->Render(projection);
	}
}
