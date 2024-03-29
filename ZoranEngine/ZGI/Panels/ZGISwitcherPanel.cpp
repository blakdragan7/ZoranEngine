#include "stdafx.h"
#include "ZGISwitcherPanel.h"
#include <Utils/VectorAddons.hpp>

ZGISwitcherPanel::ZGISwitcherPanel(ZGIVirtualWindow* owningWindow) : activeWidget(-1), ZGIPanel(owningWindow)
{
	widgets = new std::vector<ZGIWidget*>;
}


ZGISwitcherPanel::~ZGISwitcherPanel()
{
	delete widgets;
}

void ZGISwitcherPanel::AddWidget(ZGIWidget * widget)
{
	// make the widget the size of this panel
	widget->SetPosition(position);
	widget->SetSize(size);

	widgets->push_back(widget);
	if (activeWidget == -1)activeWidget = 0;
}

void ZGISwitcherPanel::RemoveWidget(ZGIWidget * widget)
{
	if (activeWidget != -1)
	{
		if ((*widgets)[activeWidget] == widget)
			--activeWidget;
	}
	remove(*widgets, widget);
}

void ZGISwitcherPanel::SetWidgetActive(ZGIWidget * widget)
{
	for (size_t i = 0; i < widgets->size(); i++)
	{
		if ((*widgets)[i] == widget)
		{
			activeWidget = static_cast<int>(i);
			break;
		}
	}

	Log(LogLevel_Warning, "Could Not Find Widget to set active\n");
}

void ZGISwitcherPanel::SetIndexActive(int index)
{
	if((int)widgets->size() > index)
		Log(LogLevel_Warning, "Invalid index to set active\n");
	activeWidget = index;
}

void ZGISwitcherPanel::AnimateAllWidgets(float dt)
{
	if (activeWidget != -1)
		(*widgets)[activeWidget]->Animate(dt);
}

bool ZGISwitcherPanel::ContainsWidget(ZGIWidget * widget) const
{
	return std::find(widgets->begin(),widgets->end(),widget) != widgets->end();
}

bool ZGISwitcherPanel::CanAddWidget(ZGIWidget * widget) const
{
	return ContainsWidget(widget) == false;
}

int ZGISwitcherPanel::GetNumberOfWidgets() const
{
	return static_cast<int>(widgets->size());
}

int ZGISwitcherPanel::GetMaxNumberOfWidgets() const
{
	return INT_MAX;
}

ZGIWidget * ZGISwitcherPanel::WidgetForPosition(Vec2D pos)
{
	return (*widgets)[activeWidget]->HitTest(pos);
}

void ZGISwitcherPanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Ancher stuff
}

void ZGISwitcherPanel::Render(const Matrix44 & projection)
{
	if (isDirty)
	{
		for (auto &w : *widgets)
		{
			w->SetSize(size);
			w->SetPosition(position);
		}
	}

	ZGIPanel::Render(projection);

	if(activeWidget != -1)
		(*widgets)[activeWidget]->Render(projection);
}
