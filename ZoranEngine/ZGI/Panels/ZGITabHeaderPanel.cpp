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

	int size = headers->size();

	for (int i = 0; i < size; i++)
	{
		auto s = (*headers)[i];
		delete s;
	}

	delete headers;
}

void ZGITabHeaderPanel::AddHeader(ZGITabHeader * header)
{

}

void ZGITabHeaderPanel::AddHeader(ZGITabHeader * header, int index)
{
}

void ZGITabHeaderPanel::AddHeader(std::string & header)
{
	ZGITabHeader* aheader = new ZGITabHeader(header, owningWindow);

	AddHeader(aheader);
}

void ZGITabHeaderPanel::InsertHeader(int index, std::string & header)
{
	ZGITabHeader* aheader = new ZGITabHeader(header, owningWindow);

	AddHeader(aheader, index);
}

void ZGITabHeaderPanel::SetHeaderActive(int index)
{
	if (headers->size() >= index)
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

int ZGITabHeaderPanel::GetNumberOfWidgets() const
{
	return headers->size();
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
	for (auto w : *headers)
	{
		w->Render(projection);
	}

	ZGIPanel::Render(projection);
}
