#include "stdafx.h"
#include "ZGIWrapBoxPanel.h"


void ZGIWrapBoxPanel::UpdateWidgetPositions()
{
	Vector2D currentLocation = position;

	currentLocation.y += size.y - contentSize.y;

	Vector2D upperBounds = position + size;

	for (auto& widget : *widgets)
	{
		if ((currentLocation.x + contentSize.x) >= upperBounds.x)
		{
			currentLocation.x = position.x;
			currentLocation.y -= contentSize.y;
		}

		widget->SetPosition(currentLocation);
		widget->SetSize(contentSize);

		currentLocation.x += contentSize.x;
	}
}

ZGIWrapBoxPanel::ZGIWrapBoxPanel(Vec2D contentSize, ZGIVirtualWindow* owningWindow) : contentSize(contentSize), needsUpdate(false), ZGIPanel(owningWindow)
{
	widgets = new std::vector<ZGIWidget*>;
}

ZGIWrapBoxPanel::~ZGIWrapBoxPanel()
{
	delete widgets;
}

void ZGIWrapBoxPanel::AddWidget(ZGIWidget * widget)
{
	widgets->push_back(widget);
}

void ZGIWrapBoxPanel::RemoveWidget(ZGIWidget * widget)
{
	auto &itr = std::find(widgets->begin(), widgets->end(), widget);
	if (itr != widgets->end())
	{
		widgets->erase(itr);
	}
	else
	{
		Log(LogLevel_Warning, "Trying To Remove Invalid Widget");
	}
}

void ZGIWrapBoxPanel::AnimateAllWidgets(float dt)
{
	for (auto& w : *widgets)
	{
		w->Animate(dt);
	}
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
