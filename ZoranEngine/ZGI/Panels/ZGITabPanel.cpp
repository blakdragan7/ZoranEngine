#include "stdafx.h"
#include "ZGITabPanel.h"

#include <Utils/VectorAddons.hpp>

#include <ZGI/Panels/ZGISwitcherPanel.h>
#include <ZGI/Panels/ZGITabHeaderPanel.h>
#include <ZGI/Panels/ZGIVerticalBoxPanel.h>

ZGITabPanel::ZGITabPanel(ZGIVirtualWindow* owningWindow) : ZGIPanel(owningWindow)
{
	body = new ZGISwitcherPanel(owningWindow);
	header = new ZGITabHeaderPanel(owningWindow);
	panel = new ZGIVerticalBoxPanel(owningWindow);

	panel->AddWidget(header);
	panel->AddWidget(body);
}

ZGITabPanel::~ZGITabPanel()
{
	delete body;
}

void ZGITabPanel::AddTab(const std::string& name, ZGIWidget * widget)
{
	header->AddHeader(name);
	body->AddWidget(widget);
}

void ZGITabPanel::RemoveTab(const std::string& name)
{
	int index =  header->IndexForName(name);
	if (index != -1)
	{
		header->RemoveHeaderForIndex(index);
		body->RemoveWidget(index);
	}
	else
	{
		LOG_ERROR << "ZGITabPanel::RemoveTab Could not remove tab for name " << name << std::endl;
	}
}

void ZGITabPanel::RemoveTab(int index)
{
	body->RemoveWidget(index);
	header->RemoveHeaderForIndex(index);
}

void ZGITabPanel::SetIndexActive(int index)
{
	header->SetHeaderActive(index);
	body->SetIndexActive(index);
}

void ZGITabPanel::RemoveWidget(ZGIWidget *widget)
{
	int index = body->IndexForWidget(widget);

	if (index == -1)
	{
		LOG_ERROR << "ZGITabPanel::RemoveWidget(ZGIWidget *widget) Could not Remove Widget" << std::endl;
		return;
	}

	RemoveTab(index);
}

void ZGITabPanel::AnimateAllWidgets(float dt)
{
	body->AnimateAllWidgets(dt);
}

bool ZGITabPanel::ContainsWidget(ZGIWidget * widget) const
{
	return body->ContainsWidget(widget);
}

bool ZGITabPanel::CanAddWidget(ZGIWidget * widget) const
{
	return body->CanAddWidget(widget);
}

int ZGITabPanel::GetNumberOfWidgets() const
{
	return body->GetNumberOfWidgets();
}

int ZGITabPanel::GetMaxNumberOfWidgets() const
{
	return INT_MAX;
}

ZGIWidget * ZGITabPanel::WidgetForPosition(Vec2D pos)
{
	return body->WidgetForPosition(pos);
}

void ZGITabPanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	body->ContainerResized(newSize, oldSize);
}

void ZGITabPanel::Render(const Matrix44 & projection)
{
	if (isDirty)
	{
		panel->SetSize(size);
		panel->SetPosition(position);
	}

	panel->Render(projection);

	ZGIPanel::Render(projection);
}
