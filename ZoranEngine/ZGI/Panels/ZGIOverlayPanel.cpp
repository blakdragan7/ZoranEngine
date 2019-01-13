#include "stdafx.h"
#include "ZGIOverlayPanel.h"

void ZGIOverlayPanel::UpdateSockets()
{
	for (auto&s : *sockets)
	{
		s.content->SetSize(size);
		s.content->SetPosition(position);
	}
}

ZGIOverlayPanel::ZGIOverlayPanel(ZGIVirtualWindow* owningWindow) : ZGIPanel(owningWindow), needsSocketUpdate(false)
{
	sockets = new std::vector<ZGIBasicSocket>;
}

ZGIOverlayPanel::~ZGIOverlayPanel()
{
	delete sockets;
}

void ZGIOverlayPanel::MoveWidgetForward(ZGIWidget * widget)
{
	auto& itr = std::find(sockets->begin(), sockets->end(), widget);
	if (itr == sockets->end())
	{
		Log(LogLevel_Warning, "Trying To Move Invalid Widget!\n");
		return;
	}

	auto nextItr = itr + 1;
	if (nextItr == sockets->end())
		std::rotate(sockets->begin(), sockets->end() - 1, sockets->end());
	else
		std::iter_swap(itr, nextItr);
}

void ZGIOverlayPanel::MoveWidgetBackward(ZGIWidget * widget)
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
}

void ZGIOverlayPanel::AddWidget(ZGIWidget * widget)
{
	sockets->push_back({ widget,this });
	needsSocketUpdate = true;
}

void ZGIOverlayPanel::RemoveWidget(ZGIWidget * widget)
{
	auto& itr = std::find(sockets->begin(), sockets->end(), widget);
	if (itr != sockets->end())
	{
		sockets->erase(itr);
	}
}

void ZGIOverlayPanel::AnimateAllWidgets(float dt)
{
	for (auto& w : *sockets)
	{
		w.content->Animate(dt);
	}
}

bool ZGIOverlayPanel::ContainsWidget(ZGIWidget * widget) const
{
	return std::find(sockets->begin(),sockets->end(), widget) != sockets->end();
}

bool ZGIOverlayPanel::CanAddWidget(ZGIWidget * widget) const
{
	return ContainsWidget(widget) == false;
}

int ZGIOverlayPanel::GetNumberOfWidgets() const
{
	return static_cast<int>(sockets->size());
}

int ZGIOverlayPanel::GetMaxNumberOfWidgets() const
{
	return INT_MAX;
}

ZGIWidget * ZGIOverlayPanel::WidgetForPosition(Vec2D pos)
{
	for (auto& s : *sockets)
	{
		if (auto w = s.content->HitTest(pos))return w;
	}

	return nullptr;
}

void ZGIOverlayPanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Ancher Stuff
}

void ZGIOverlayPanel::Render(const Matrix44 & projection)
{
	if (isDirty || needsSocketUpdate)
	{
		UpdateSockets();
		needsSocketUpdate = false;
	}

	ZGIPanel::Render(projection);
	
	for (auto& s : *sockets)
	{
		s.content->Render(projection);
	}
}
