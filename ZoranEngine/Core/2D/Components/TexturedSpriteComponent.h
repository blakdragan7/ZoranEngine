#pragma once
#include <Core\2D\Components\Visible2DComponent.h>
#include <Rendering/RenderTypes.h>

class TextureBase;
class ZoranEngine_EXPORT TexturedSpriteComponent : public Visible2DComponent
{
private:
	TextureBase * texture;

public:
	TexturedSpriteComponent();
	TexturedSpriteComponent(const char* texture, RenderDataType type, RenderDataFormat format);
	~TexturedSpriteComponent();

	virtual void PreRender()override;

	void SetTexture(const char* texture, RenderDataType type, RenderDataFormat format);
};

