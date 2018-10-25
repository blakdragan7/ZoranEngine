#pragma once
#include <ZGI\ZGIWidget.h>

class TextureBase;
class GUIRenderer;
class ZGIImageWidget : public ZGIWidget
{
private:
	TextureBase * image;
	GUIRenderer* renderer;

public:
	ZGIImageWidget();
	~ZGIImageWidget();

	inline TextureBase* GetImage() { return image; }

	void SetImage(const char* image);
	void SetImage(TextureBase* image);

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
	virtual void Render(const Matrix44& projection)override;

};

