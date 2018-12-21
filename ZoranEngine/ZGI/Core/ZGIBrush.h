#pragma once

#include <Rendering/Primitives.h>

class GUIRenderer;
class TextureBase;
class Matrix44;
class ZoranEngine_EXPORT ZGIBrush
{
private:
	GUIRenderer* renderer;
	TextureBase* backgroundImage;

public:
	ZGIBrush();
	~ZGIBrush();

	inline const TextureBase* GetBackgroundImage()const { return backgroundImage; }

	void SetBackgroudImage(std::string texture);
	void SetBackgroudImage(TextureBase* texture);
	void SetBackgroudHue(const Color& hue);

	void RenderBrush(const Matrix44& matrix);
};

