#pragma once
#include <ZGI/Widgets/ZGIButton.h>
#include <Resources/FontResource.h>

class ZGILabel;
class ZGILabelButton : public ZGIButton
{
private:
	ZGILabel* label;

public:
	// uses default font resource
	ZGILabelButton(ZGIVirtualWindow* owningWindow);
	ZGILabelButton(FontResource font, ZGIVirtualWindow* owningWindow);
	~ZGILabelButton();

	void SetText(std::string& text);
	void SetAlignment(Alignment textAligment);
};

