#pragma once

#include <Rendering/Primitives.h>

#include <Resources/ImageResource.h>

class GUIRenderer;
class Matrix44;
class ZoranEngine_EXPORT ZGIBrush
{
private:
	GUIRenderer* renderer;
	ImageResource backgroundImage;

public:
	ZGIBrush();
	~ZGIBrush();

	inline const ImageResource GetBackgroundImage()const { return backgroundImage; }

	void SetBackgroudImage(const char* texture);
	void SetBackgroudImage(std::string texture);
	void SetBackgroudImage(ImageResource texture);
	void SetBackgroudHue(const Color& hue);

	void RenderBrush(const Matrix44& matrix);
};

