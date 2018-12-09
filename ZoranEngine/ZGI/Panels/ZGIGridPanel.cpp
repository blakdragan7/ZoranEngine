#include "stdafx.h"
#include "ZGIGridPanel.h"

#include <Utils/VectorAddons.hpp>
#include <Core/CommomTypes.h>

void ZGIGridPanel::RepositionContents()
{
	Vector2D gridSize(this->gridSize);

	for (GridSocket& sock : *grid)
	{
		Vector2D sockSize = (Vector2D(sock.gridSize) / gridSize) * size;
		Vector2D sockBottomLeft = position + ((Vector2D(sock.gridPosition) / gridSize) * size);

		sock.widget->SetPosition(sockBottomLeft);
		sock.widget->SetSize(sockSize);
	}
}

std::vector<GridSocket>::iterator ZGIGridPanel::Find(ZGIWidget * widget)const
{
	for (auto itr = grid->begin(); itr != grid->end(); itr++)
	{
		if (itr->widget == widget)return itr;
	}

	return grid->end();
}

GridSocket * ZGIGridPanel::At(int x, int y)
{
	auto itr = std::find(grid->begin(), grid->end(), Vector2I(x, y));

	if (itr == grid->end())return false;

	return &(*itr);
}

ZGIGridPanel::ZGIGridPanel(int sizeX, int sizeY, ZGIVirtualWindow * owningWindow) : numWidgets(0), ZGIPanel(owningWindow)
{
	grid = new std::vector<GridSocket>;
	gridSize.x = sizeX;
	gridSize.y = sizeY;
}

ZGIGridPanel::ZGIGridPanel(Vec2I size, ZGIVirtualWindow * owningWindow) : ZGIPanel(owningWindow)
{
	grid = new std::vector<GridSocket>;
	this->gridSize = size;
}

ZGIGridPanel::~ZGIGridPanel()
{
	for (auto w : *grid)
	{
		if(w.widget)delete w.widget;
	}

	delete grid;
}

void ZGIGridPanel::Resize(int sizeX, int sizeY)
{
	gridSize.x = sizeX;
	gridSize.y = sizeY;
}

void ZGIGridPanel::Resize(Vec2I newSize)
{
	gridSize = newSize;
}

const GridSocket* ZGIGridPanel::At(int x, int y) const
{
	auto itr = std::find(grid->begin(),grid->end(), Vector2I(x, y));

	if (itr == grid->end())return false;

	return &(*itr);
}

bool ZGIGridPanel::KeyEventSub(KeyEventType type, unsigned key)
{
	return false;
}

bool ZGIGridPanel::PositionIsEmpty(Vec2I position) const
{
	auto itr = std::find(grid->begin(), grid->end(), position);
	return itr == grid->end();
}

bool ZGIGridPanel::ContainsWidget(ZGIWidget * widget) const
{
	return Find(widget) != grid->end();
}

bool ZGIGridPanel::CanAddWidget(ZGIWidget * widget) const
{
	return  ContainsWidget(widget) == false;
}

bool ZGIGridPanel::AddWidget(ZGIWidget * widget, int x, int y, int rowSpan, int columnSpan, AlignmentBit alignment)
{
	if (CanAddWidget(widget) == false || PositionIsEmpty({ x,y }) == false)return false;

	GridSocket sock({ x,y }, {rowSpan,columnSpan}, alignment, widget);

	grid->push_back(sock);

	return true;
}

bool ZGIGridPanel::RemoveWidget(ZGIWidget * widget)
{
	auto itr = Find(widget);
	if (itr == grid->end())return false;
	return remove(*grid, itr);
}

bool ZGIGridPanel::RemoveWidget(int x, int y)
{
	auto itr = std::find(grid->begin(), grid->end(), Vector2I( x,y ));
	if (itr == grid->end())return false;
	return remove(*grid, itr);
}

int ZGIGridPanel::GetNumberOfWidgets() const
{
	return static_cast<int>(grid->size());
}

int ZGIGridPanel::GetMaxNumberOfWidgets() const
{
	return INT_MAX;
}

ZGIWidget * ZGIGridPanel::WidgetForPosition(Vec2D pos)
{
	for (auto& w : *grid)
	{
		if (ZGIWidget* widget = w.widget->HitTest(pos))
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
		w.widget->Render(projection);

	ZGIPanel::Render(projection);
}

void ZGIGridPanel::Print(unsigned tabs) const
{
	ZGIWidget::Print(tabs);

	for (auto s : *grid)
	{
		s.widget->Print(tabs+1);
	}
}
