#pragma once
#include<ZGI\Widgets\ZGIWidget.h>

class FontRenderer;
class Font;
class ZGILabelWidget : public ZGIWidget
{
private:
	FontRenderer * renderer;

	Font* text;

public:
	ZGILabelWidget(ZGIVirtualWindow* owningWindow);
	~ZGILabelWidget();

	void SetText(const std::string& text);

	virtual void Render(const Matrix44& projection)override;
};

