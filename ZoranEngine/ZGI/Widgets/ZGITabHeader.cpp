#include "stdafx.h"
#include "ZGITabHeader.h"

#include <ZGI/Widgets/ZGILabelButton.h>


ZGITabHeader::ZGITabHeader(std::string& headerText, ZGIVirtualWindow* owningWindow) : ZGIWidget(owningWindow)
{
	button = new ZGILabelButton(owningWindow);
	button->SetText(headerText);
	isActive = false;
}

ZGITabHeader::ZGITabHeader(std::string & headerText, FontResource font, ZGIVirtualWindow * owningWindow) : ZGIWidget(owningWindow)
{
	button = new ZGILabelButton(font, owningWindow);
	button->SetText(headerText);
	isActive = false;
}

ZGITabHeader::~ZGITabHeader()
{
	delete button;
}

void ZGITabHeader::SetHeaderActive(bool active)
{
	if (active)
		button->SetStandardColor(selectedColor);
	else
		button->SetStandardColor(notSelectedColor);

	isActive = active;
}

void ZGITabHeader::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Ancher stuff
}

ZGIWidget * ZGITabHeader::HitTest(Vec2D pos)
{
	return button->HitTest(pos);
}

void ZGITabHeader::Render(const Matrix44 & projection)
{
	button->Render(projection);

	ZGIWidget::Render(projection);
}
