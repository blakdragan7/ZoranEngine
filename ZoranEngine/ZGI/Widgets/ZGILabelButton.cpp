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

void ZGILabelButton::SetText(const std::string & text)
{
	label->SetText(text);
	stringCopy = text;
}

void ZGILabelButton::SetAlignment(Alignment textAligment)
{
	label->SetAlignment(textAligment);
}

const std::string & ZGILabelButton::GetText()
{
	return stringCopy;
}
