#include "stdafx.h"
#include "ZGIFreeFormPanel.h"

#include <Utils/VectorAddons.hpp>

ZGIFreeFormPanel::ZGIFreeFormPanel(ZGIVirtualWindow* owningWindow) : ZGIPanel(owningWindow)
{
	widgets = new std::vector<ZGIWidget*>;
}

ZGIFreeFormPanel::~ZGIFreeFormPanel()
{
	delete widgets;
}

void ZGIFreeFormPanel::AnimateAllWidgets(float dt)
{
	for (auto& w : *widgets)
	{
		w->Animate(dt);
	}
}

bool ZGIFreeFormPanel::ContainsWidget(ZGIWidget * widget) const
{
	auto itr = find(*widgets, widget);

	return itr != widgets->end();
}

bool ZGIFreeFormPanel::CanAddWidget(ZGIWidget * widget) const
{
	return true;
}

void ZGIFreeFormPanel::AddWidget(ZGIWidget * widget)
{
	widgets->push_back(widget);
}

void ZGIFreeFormPanel::RemoveWidget(ZGIWidget * widget)
{
	remove(*widgets, widget);
}

int ZGIFreeFormPanel::GetNumberOfWidgets() const
{
	return static_cast<int>(widgets->size());
}

int ZGIFreeFormPanel::GetMaxNumberOfWidgets() const
{
	return INT_MAX;
}

ZGIWidget * ZGIFreeFormPanel::WidgetForPosition(Vec2D pos)
{
	for (auto widget : *widgets)
	{
		if (ZGIWidget* h = widget->HitTest(pos))
		{
			return h;
		}
	}

	return nullptr;
}

void ZGIFreeFormPanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: ancher stuff
}

void ZGIFreeFormPanel::Render(const Matrix44 & projection)
{
	for (auto widget : *widgets)
	{
		widget->Render(projection);
	}
}

void ZGIFreeFormPanel::Print(unsigned tabs) const
{
	ZGIWidget::Print(tabs);

	for (auto w : *widgets)
		w->Print(tabs+1);
}
