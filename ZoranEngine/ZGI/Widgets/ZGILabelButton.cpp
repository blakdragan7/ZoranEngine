#include "stdafx.h"
#include "ZGILabelButton.h"
#include <ZGI/Widgets/ZGILabel.h>

ZGILabelButton::ZGILabelButton(ZGIVirtualWindow* owningWindow) : ZGIButton(owningWindow)
{
	label = new ZGILabel(owningWindow);
	label->SetBoundsFromSize(true);
	label->SetAlignment(Alignment_Center);
}

ZGILabelButton::ZGILabelButton(FontResource font, ZGIVirtualWindow* owningWindow) : ZGIButton(owningWindow)
{
	label = new ZGILabel(font, owningWindow);
	label->SetBoundsFromSize(true);
	label->SetAlignment(Alignment_Center);
}

ZGILabelButton::~ZGILabelButton()
{
	delete label;
}

void ZGILabelButton::SetText(std::string & text)
{
	label->SetText(text);
}

void ZGILabelButton::SetAlignment(Alignment textAligment)
{
	label->SetAlignment(textAligment);
}
