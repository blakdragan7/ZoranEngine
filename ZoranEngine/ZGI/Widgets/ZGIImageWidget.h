#pragma once
#include <ZGI\Widgets\ZGIWidget.h>

class TextureBase;
class GUIRenderer;
class ZoranEngine_EXPORT ZGIImageWidget : public ZGIWidget
{
public:
	ZGIImageWidget(ZGIVirtualWindow* owningWindow);
	~ZGIImageWidget();

	inline void SetSizeToImage() { SetSize(bounds); }

	void SetImage(const char* image);
	void SetImage(TextureBase* image);

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
};

