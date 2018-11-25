#pragma once

#include <Rendering/Primitives.h>

class GUIRenderer;
class TextureBase;
class Matrix44;
class ZGIBrush
{
private:
	GUIRenderer* renderer;
	TextureBase* backgroundImage;

public:
	ZGIBrush();
	~ZGIBrush();

	void SetBackgroudImage(TextureBase* texture);
	void SetBackgroudHue(const Color& hue);

	void RenderBrush(const Matrix44& matrix);
};

