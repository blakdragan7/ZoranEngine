#include "stdafx.h"
#include "ZGITabHeaderPanel.h"


#include <ZGI/Panels/ZGIHorizontalBoxPanel.h>
#include <ZGI/Widgets/ZGIButton.h>
#include <ZGI/Widgets/ZGITabHeader.h>
#include <ZGI/Core/ZGIBrush.h>

ZGITabHeaderPanel::ZGITabHeaderPanel(ZGIVirtualWindow* owningWindow) : ZGIPanel(owningWindow)
{
	headers = new std::vector<ZGITabHeader*>;
	activeHeader = -1;
}

ZGITabHeaderPanel::~ZGITabHeaderPanel()
{
	// delete everything owned by innerPannel

	int size = (int)headers->size();

	for (int i = 0; i < size; i++)
	{
		auto s = (*headers)[i];
		delete s;
	}

	delete headers;
}

void ZGITabHeaderPanel::AddHeader(ZGITabHeader * header)
{
	headers->push_back(header);
}

void ZGITabHeaderPanel::AddHeader(ZGITabHeader * header, int index)
{
	if (index < headers->size() && index >= 0)
		headers->insert(headers->begin() + index, header);
	else
	{
		LOG_ERROR << "ZGITabHeaderPanel::AddHeader index: " << index << " out of range. Appending to end instead !" << std::endl;
		headers->push_back(header);
	}
}

void ZGITabHeaderPanel::AddHeader(const std::string & header)
{
	ZGITabHeader* aheader = new ZGITabHeader(header, owningWindow);

	AddHeader(aheader);
}

void ZGITabHeaderPanel::InsertHeader(int index, const std::string & header)
{
	ZGITabHeader* aheader = new ZGITabHeader(header, owningWindow);

	AddHeader(aheader, index);
}

void ZGITabHeaderPanel::SetHeaderActive(int index)
{
	if (headers->size() < index)
	{
		LOG_ERROR << "ZGITabHeaderPanel::SetHeaderActive: Index out of bounds" << std::endl;

		return;
	}

	if (activeHeader != -1)
	{
		ZGITabHeader* header = (*headers)[index];
		header->SetHeaderActive(false);
	}

	activeHeader = index;
	ZGITabHeader* header = (*headers)[index];
	header->SetHeaderActive(true);
}

int ZGITabHeaderPanel::GetActiveIndex() const
{
	return activeHeader;
}

void ZGITabHeaderPanel::RemoveHeaderForIndex(int index)
{
	if (index >= headers->size())
	{
		LOG_ERROR << "ZGITabHeaderPanel::RemoveHeaderForIndex out of bounds !" << std::endl;
		return;
	}

	auto itr = headers->begin() + index;
	delete *itr;
	headers->erase(itr);
}

void ZGITabHeaderPanel::RemoveHeaderNoDelete(ZGITabHeader* header)
{
	auto itr = std::find(headers->begin(), headers->end(), header);
	if (itr == headers->end())
	{
		LOG_ERROR << "ZGITabHeaderPanel::RemoveHeaderNoDelete Could Not Find Header To Remove!!" << std::endl;
		return;
	}

	headers->erase(itr);
}

int ZGITabHeaderPanel::IndexForName(const std::string & name) const
{
	auto itr = std::find(headers->begin(), headers->end(), name);
	if (itr == headers->end())
	{
		LOG_ERROR << "ZGITabHeaderPanel::IndexForName Could Not Find Header for Name: " << name << std::endl;
		return -1;
	}

	return (int)(itr - headers->begin());
}

ZGITabHeader * ZGITabHeaderPanel::RemoveHeaderNoDelete(int index)
{
	auto itr = headers->begin() + index;
	auto w = *itr;
	headers->erase(itr);
	
	return w;
}

void ZGITabHeaderPanel::AnimateAllWidgets(float dt)
{
	for (auto& s : *headers)
	{
		s->Animate(dt);
	}
}

bool ZGITabHeaderPanel::ContainsWidget(ZGIWidget * widget) const
{
	return std::find(headers->begin(), headers->end(), widget) != headers->end();
}

bool ZGITabHeaderPanel::CanAddWidget(ZGIWidget * widget) const
{
	return ContainsWidget(widget);
}

void ZGITabHeaderPanel::RemoveWidget(ZGIWidget * widget)
{
	auto itr = std::find(headers->begin(), headers->end(), widget);
	if (itr != headers->end())
	{
		headers->erase(itr);
	}
}

int ZGITabHeaderPanel::GetNumberOfWidgets() const
{
	return (int)headers->size();
}

int ZGITabHeaderPanel::GetMaxNumberOfWidgets() const
{
	return INT_MAX;
}

ZGIWidget * ZGITabHeaderPanel::WidgetForPosition(Vec2D pos)
{
	for (auto& s : *headers)
	{
		if (auto w = s->HitTest(pos))return w;
	}

	return nullptr;
}

void ZGITabHeaderPanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Ancher stuff
}

void ZGITabHeaderPanel::Render(const Matrix44 & projection)
{
	if (isDirty)
	{
		for (size_t i=0;i<headers->size();i++)
		{
			ZGITabHeader* w = (*headers)[i];
			w->SetSize({size.x / 6.0f, size.y});
			w->SetPosition({position.x + (i * size.x / 6.0f), position.y});
		}
	}

	for (auto w : *headers)
	{
		w->Render(projection);
	}

	ZGIPanel::Render(projection);
}
