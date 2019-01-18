#include "stdafx.h"
#include "ZGIListPanel.h"

#include <Utils/VectorAddons.hpp>

void ZGIListPanel::PositionAndSizeWidgets()
{
	// TODO: actualy use alignment

	Vector2D widgetSize(size.w, size.h / maxListSize);

	for (auto index = 0;index < widgetList->size(); index++)
	{
		auto& s = (*widgetList)[index];

		Vector2D widgetPosition(position.x,(position.y + size.h - widgetSize.h) - (index * widgetSize.h));

		s.widget->SetSize(widgetSize);
		s.widget->SetPosition(widgetPosition);
	}
}

ZGIListPanel::ZGIListPanel(bool dynamicSize, ZGIVirtualWindow* owningWindow) : dynamicSize(dynamicSize), maxListSize(10), ZGIPanel(owningWindow)
{
	widgetList = new std::vector<ListSocket>;
}


ZGIListPanel::~ZGIListPanel()
{
	delete widgetList;
}

void ZGIListPanel::AddWidget(ZGIWidget * widget, AlignmentBit alignment)
{
	widget->SetParent(this);
	widgetList->push_back({ widget,alignment });
}

void ZGIListPanel::RemoveWidget(ZGIWidget * widget)
{
	auto itr = std::find(widgetList->begin(), widgetList->end(), widget);
	if (itr != widgetList->end())
	{
		widgetList->erase(itr);
		widget->SetParent(0);
	}
}

void ZGIListPanel::Print(unsigned tabs)const
{
	ZGIWidget::Print(tabs);

	for (auto& w : *widgetList)
	{
		w.widget->Print(tabs + 1);
	}
}

void ZGIListPanel::AnimateAllWidgets(float dt)
{
	for (auto& w : *widgetList)
	{
		if (w.widget)w.widget->Animate(dt);
	}
}

bool ZGIListPanel::ContainsWidget(ZGIWidget * widget) const
{
	return std::find(widgetList->begin(), widgetList->end(), widget) != widgetList->end();
}

bool ZGIListPanel::CanAddWidget(ZGIWidget * widget) const
{
	return ContainsWidget(widget) == false;
}

int ZGIListPanel::GetNumberOfWidgets() const
{
	return static_cast<int>(widgetList->size());
}

int ZGIListPanel::GetMaxNumberOfWidgets() const
{
	return INT_MAX;
}

ZGIWidget * ZGIListPanel::WidgetForPosition(Vec2D pos)
{
	for (auto s : *widgetList)
	{
		if (ZGIWidget* widget = s.widget->HitTest(pos))
		{
			return widget;
		}
	}

	return nullptr;
}

void ZGIListPanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: ancher stuff
}

void ZGIListPanel::Render(const Matrix44 & projection)
{
	if (isDirty)
	{
		if (dynamicSize)maxListSize = static_cast<unsigned int>(widgetList->size());
		PositionAndSizeWidgets();
	}

	for (auto s : *widgetList)
	{
		s.widget->Render(projection);
	}

	ZGIPanel::Render(projection);
}
