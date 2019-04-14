#pragma once
#include <ZGI/Widgets/ZGIButton.h>
#include <Resources/FontResource.h>

class ZGILabel;
class ZGILabelButton : public ZGIButton
{
private:
	ZGILabel* label;
	std::string stringCopy; // copy of label string
public:
	// uses default font resource
	ZGILabelButton(ZGIVirtualWindow* owningWindow);
	ZGILabelButton(FontResource font, ZGIVirtualWindow* owningWindow);
	~ZGILabelButton();

	void SetText(const std::string& text);
	void SetAlignment(Alignment textAligment);
	const std::string& GetText();
};

