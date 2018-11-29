#include "stdafx.h"
#include "ZGIGridPanel.h"

#include <Utils/VectorAddons.hpp>

void ZGIGridPanel::RepositionContents()
{
	for (int index = 0;index < grid->size();index++)
	{

	}
}

ZGIGridPanel::ZGIGridPanel(int sizeX, int sizeY, ZGIVirtualWindow * owningWindow) : numWidgets(0), ZGIPanel(owningWindow)
{
	grid = new std::vector<ZGIWidget*>(sizeX * sizeY);
	size.x = sizeX;
	size.y = sizeY;
}

ZGIGridPanel::ZGIGridPanel(Vec2I size, ZGIVirtualWindow * owningWindow) : ZGIPanel(owningWindow)
{
	grid = new std::vector<ZGIWidget*>(size.x * size.y);
	this->size = size;
}

ZGIGridPanel::~ZGIGridPanel()
{
	for (auto w : *grid)
	{
		if(w)delete w;
	}

	delete grid;
}

void ZGIGridPanel::Resize(int sizeX, int sizeY)
{
	grid->resize(sizeX*sizeY);
	size.x = sizeX;
	size.y = sizeY;
}

void ZGIGridPanel::Resize(Vec2I newSize)
{
	grid->resize(newSize.x*newSize.y);
	size = newSize;
}

ZGIWidget * ZGIGridPanel::At(int x, int y) const
{
	int index = IndexOf(x, y);
	
	if (grid->size() <= index)
	{
		return false;
	}

	return (*grid)[index];
}

bool ZGIGridPanel::KeyEventSub(KeyEventType type, unsigned key)
{
	return false;
}

bool ZGIGridPanel::PositionIsEmpty(Vec2I position) const
{
	int index = IndexOf(position);
	if (grid->size() <= index)return false;
	return (*grid)[index] == 0;
}

bool ZGIGridPanel::ContainsWidget(ZGIWidget * widget) const
{
	return find(*grid,widget) != grid->end();
}

bool ZGIGridPanel::CanAddWidget(ZGIWidget * widget) const
{
	return  ContainsWidget(widget) == false;
}

bool ZGIGridPanel::AddWidget(ZGIWidget * widget, int x, int y)
{
	if (CanAddWidget == false)return false;

	int index = IndexOf(x, y);

	if (grid->size() <= index)
	{
		return false;
	}

	if ((*grid)[index])delete (*grid)[index];

	(*grid)[index] = widget;

	if (widget)numWidgets++;
	else numWidgets--;
}

int ZGIGridPanel::GetNumberOfWidgets() const
{
	return numWidgets;
}

int ZGIGridPanel::GetMaxNumberOfWidgets() const
{
	return size.x * size.y;
}

ZGIWidget * ZGIGridPanel::WidgetForPosition(Vec2D pos)
{
	for (auto& w : *grid)
	{
		if (ZGIWidget* widget = w->HitTest(pos))
				return widget;
	}

	return nullptr;
}

void ZGIGridPanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: ancher stuff
}

void ZGIGridPanel::Render(const Matrix44 & projection)
{
	if (isDirty)
		RepositionContents();
	
	for (auto& w : *grid)
		w->Render(projection);

	ZGIPanel::Render(projection);
}
