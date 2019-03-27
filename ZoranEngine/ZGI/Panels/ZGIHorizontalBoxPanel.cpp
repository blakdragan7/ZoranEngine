#include "stdafx.h"
#include "ZGIHorizontalBoxPanel.h"


void ZGIHorizontalBoxPanel::UpdateSocketPositions()
{
	float width = size.w;
	float height = size.h;

	Vector2D sub_position = position;

	for (auto& s : *sockets)
	{
		Vector2D subSize(width * s.widthPercent, height);
		s.content->SetSize(subSize);
		s.content->SetPosition(sub_position);

		sub_position.x += subSize.w;
	}
}

ZGIHorizontalBoxPanel::ZGIHorizontalBoxPanel(ZGIVirtualWindow* owningWindow) : ZGIPanel(owningWindow), needsSocketsSized(false)
{
	sockets = new std::vector<ZGIBasicSocket>();
}

ZGIHorizontalBoxPanel::~ZGIHorizontalBoxPanel()
{
	delete sockets;
}

void ZGIHorizontalBoxPanel::MoveWidgetRight(ZGIWidget * widget)
{
	auto& itr = std::find(sockets->begin(), sockets->end(), widget);
	if (itr == sockets->end())
	{
		Log(LogLevel_Warning, "Trying To Move Invalid Widget!\n");
		return;
	}

	auto nextItr = itr + 1;
	if (nextItr == sockets->end())
		std::rotate(sockets->begin(), sockets->end()-1, sockets->end());
	else
		std::iter_swap(itr, nextItr);

	needsSocketsSized = true;
}

void ZGIHorizontalBoxPanel::MoveWidgetLeft(ZGIWidget * widget)
{
	auto& itr = std::find(sockets->begin(), sockets->end(), widget);
	if (itr == sockets->end())
	{
		Log(LogLevel_Warning, "Trying To Move Invalid Widget!\n");
		return;
	}

	if (itr == sockets->begin())
	{
		std::rotate(sockets->begin(), sockets->begin() + 1, sockets->end());
	}
	else
	{
		auto nextItr = itr - 1;

		std::iter_swap(itr, nextItr);
	}

	needsSocketsSized = true;
}

void ZGIHorizontalBoxPanel::SetSizeForWidget(float percentage, ZGIWidget * widget)
{

	auto& itr = std::find(sockets->begin(), sockets->end(), widget);

	if (itr == sockets->end())
	{
		Log(LogLevel_Warning, "Trying to set Percentage for Invalid widget !\n");
		return;
	}

	itr->width = percentage;
	needsSocketsSized = true;
}

void ZGIHorizontalBoxPanel::SetSizeForIndex(float percentage, int index)
{
	if (sockets->size() <= index)
	{
		Log(LogLevel_Warning, "Trying to set Percentage for Invalid Position !\n");
		return;
	}

	auto& socket = (*sockets)[index];

	socket.width = percentage;
	needsSocketsSized = true;
}

void ZGIHorizontalBoxPanel::CommitSizes()
{
	float totalWidth = 0.0f;

	for (auto &s : *sockets)
	{
		totalWidth += s.width;
	}

	for (auto &s : *sockets)
	{
		s.widthPercent = s.width / totalWidth;
	}
}

void ZGIHorizontalBoxPanel::AddWidget(ZGIWidget * widget, float size, int index)
{
	widget->SetParent(this);
	if (index == -1 || index >= sockets->size())
		sockets->push_back({ size, 1.0f, widget,this });
	else
		sockets->insert(sockets->begin() + index, { size, 1.0f, widget, this });
	
	needsSocketsSized = true;
}

void ZGIHorizontalBoxPanel::RemoveWidget(ZGIWidget * widget)
{
	auto& itr = std::find(sockets->begin(), sockets->end(), widget);
	if (itr != sockets->end())
	{
		widget->SetParent(0);
		sockets->erase(itr);
	}
}

ZGIWidget* ZGIHorizontalBoxPanel::RemoveWidget(int index)
{
	if (index >= sockets->size() || index < 0)
	{
		LOG_ERROR << "ZGIHorizontalBoxPanel::RemoveWidget Index out of bounds for socket !\n";
		return 0;
	}

	ZGIWidget* widget = (*sockets)[index].content;
	widget->SetParent(0);

	sockets->erase(sockets->begin() + index);
	return widget;
}

const ZGIBasicSocket * ZGIHorizontalBoxPanel::SocketForIndex(int index)const
{
	if (sockets->size() > index)
		return &((*sockets)[index]);

	return nullptr;
}

void ZGIHorizontalBoxPanel::AnimateAllWidgets(float dt)
{
	for (auto& w : *sockets)
	{
		w.content->Animate(dt);
	}
}

bool ZGIHorizontalBoxPanel::ContainsWidget(ZGIWidget * widget) const
{
	return std::find(sockets->begin(),sockets->end(), widget) != sockets->end();
}

bool ZGIHorizontalBoxPanel::CanAddWidget(ZGIWidget * widget) const
{
	return ContainsWidget(widget) == false;
}

int ZGIHorizontalBoxPanel::GetNumberOfWidgets() const
{
	return static_cast<int>(sockets->size());
}

int ZGIHorizontalBoxPanel::GetMaxNumberOfWidgets() const
{
	return INT_MAX;
}

ZGIWidget * ZGIHorizontalBoxPanel::WidgetForPosition(Vec2D pos)
{
	for (auto& s : *sockets)
	{
		if (auto w = s.content->HitTest(pos))return w;
	}

	return nullptr;
}

void ZGIHorizontalBoxPanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: ancher stuff
}

void ZGIHorizontalBoxPanel::Render(const Matrix44 & projection)
{
	if (isDirty || needsSocketsSized)
	{
		CommitSizes();
		UpdateSocketPositions();
		needsSocketsSized = false;
	}

	ZGIPanel::Render(projection);

	for (auto& s : *sockets)
	{
		s.content->Render(projection);
	}
}
