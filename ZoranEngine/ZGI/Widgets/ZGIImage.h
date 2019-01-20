#pragma once
#include <ZGI\Widgets\ZGIWidget.h>

#include <Resources/ImageResource.h>

class TextureBase;
class GUIRenderer;
class ZoranEngine_EXPORT ZGIImage : public ZGIWidget
{
private:
	ZGIBrush* foregroundBrush;
	bool hasForgroundImage;

public:
	ZGIImage(ZGIVirtualWindow* owningWindow);
	~ZGIImage();

	inline void SetSizeToImage() { SetSize(bounds); }

	void SetHue(const Color& color);
	void SetForgroundHue(const Color& color);

	void SetForegroundImage(const char* image);
	void SetForegroundImage(ImageResource image);

	void SetImage(const char* imagePath);
	void SetImage(ImageResource image);

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
	virtual const char* GetClassString()const override { return "ZGIImage"; }

	void Render(const Matrix44& projection)override;
};

