#pragma once
#include <ZGI\Widgets\ZGIWidget.h>

class TextureBase;
class GUIRenderer;
class ZoranEngine_EXPORT ZGIImage : public ZGIWidget
{
public:
	ZGIImage(ZGIVirtualWindow* owningWindow);
	~ZGIImage();

	inline void SetSizeToImage() { SetSize(bounds); }

	void SetImage(const char* image);
	void SetImage(TextureBase* image);

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
	virtual const char* GetClassString()const override { return "ZGIImage"; }
};

