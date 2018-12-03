#include "stdafx.h"
#include "ZGIListPanel.h"


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

ZGIListPanel::ZGIListPanel(ZGIVirtualWindow* owningWindow) : maxListSize(10), ZGIPanel(owningWindow)
{
	widgetList = new std::vector<ListSocket>;
}


ZGIListPanel::~ZGIListPanel()
{
	delete widgetList;
}

void ZGIListPanel::AddWidget(ZGIWidget * widget, AlignmentBit alignment)
{
	widgetList->push_back({ widget,alignment });
}

bool ZGIListPanel::KeyEventSub(KeyEventType type, unsigned key)
{
	return false;
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
		PositionAndSizeWidgets();
	}

	for (auto s : *widgetList)
	{
		s.widget->Render(projection);
	}

	ZGIPanel::Render(projection);
}
