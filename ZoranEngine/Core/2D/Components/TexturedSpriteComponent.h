#pragma once
#include <Core\2D\Components\Visible2DComponent.h>
#include <Rendering/RenderTypes.h>

#include <Resources/ImageResource.h>

class ZoranEngine_EXPORT TexturedSpriteComponent : public Visible2DComponent
{
private:
	ImageResource texture;

public:
	TexturedSpriteComponent(unsigned renderLayer);
	TexturedSpriteComponent(unsigned renderLayer, const char* texture);
	TexturedSpriteComponent(unsigned renderLayer, ImageResource texture);
	~TexturedSpriteComponent();

	virtual void PreRender()override;

	void SetTexture(const char* resourcePath);
	void SetTexture(ImageResource texture);
};

