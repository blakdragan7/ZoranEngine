#include "stdafx.h"
#include "ZGIVerticalBoxPanel.h"


void ZGIVerticalBoxPanel::UpdateSocketPositions()
{
	float width = size.w;
	float height = size.h;

	Vector2D sub_position = position;

	for (auto& s : *sockets)
	{
		Vector2D subSize(width, height * s.heightPercent);

		s.content->SetSize(subSize);
		s.content->SetPosition(sub_position);

		sub_position.y += subSize.h;
	}
}

ZGIVerticalBoxPanel::ZGIVerticalBoxPanel(ZGIVirtualWindow* owningWindow) : ZGIPanel(owningWindow), needsSocketsSized(false)
{
	sockets = new std::vector<ZGIBasicSocket>();
}

ZGIVerticalBoxPanel::~ZGIVerticalBoxPanel()
{
	delete sockets;
}

void ZGIVerticalBoxPanel::AddWidget(ZGIWidget * widget)
{
	sockets->push_back({ widget,this });
	needsSocketsSized = true;
}

void ZGIVerticalBoxPanel::SetSizeForWidget(float percentage, ZGIWidget * widget)
{

	auto& itr = std::find(sockets->begin(), sockets->end(), widget);

	if (itr == sockets->end())
	{
		Log(LogLevel_Warning, "Trying to set Percentage for Invalid widget !\n");
		return;
	}

	itr->height = percentage;
}

void ZGIVerticalBoxPanel::SetSizeForPosition(float percentage, int position)
{
	if (sockets->size() <= position)
	{
		Log(LogLevel_Warning, "Trying to set Percentage for Invalid Position !\n");
		return;
	}

	auto& socket = (*sockets)[position];

	socket.height = percentage;
}

void ZGIVerticalBoxPanel::CommitSizes()
{
	float totalHeight = 0.0f;

	for (auto &s : *sockets)
	{
		totalHeight += s.height;
	}

	for (auto &s : *sockets)
	{
		s.heightPercent = s.height / totalHeight;
	}
}

const ZGIBasicSocket * ZGIVerticalBoxPanel::SocketForPosition(int position)const
{
	if (sockets->size() > position)
		return &((*sockets)[position]);

	return nullptr;
}

bool ZGIVerticalBoxPanel::ContainsWidget(ZGIWidget * widget) const
{
	return std::find(sockets->begin(), sockets->end(), widget) != sockets->end();
}

bool ZGIVerticalBoxPanel::CanAddWidget(ZGIWidget * widget) const
{
	return ContainsWidget(widget) == false;
}

int ZGIVerticalBoxPanel::GetNumberOfWidgets() const
{
	return static_cast<int>(sockets->size());
}

int ZGIVerticalBoxPanel::GetMaxNumberOfWidgets() const
{
	return INT_MAX;
}

ZGIWidget * ZGIVerticalBoxPanel::WidgetForPosition(Vec2D pos)
{
	for (auto& s : *sockets)
	{
		if (auto w = s.content->HitTest(pos))return w;
	}

	return nullptr;
}

void ZGIVerticalBoxPanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: ancher stuff
}

void ZGIVerticalBoxPanel::Render(const Matrix44 & projection)
{
	if (isDirty || needsSocketsSized)
	{
		CommitSizes();
		UpdateSocketPositions();
		needsSocketsSized = false;
	}

	for (auto& s : *sockets)
	{
		s.content->Render(projection);
	}

	ZGIPanel::Render(projection);
}
