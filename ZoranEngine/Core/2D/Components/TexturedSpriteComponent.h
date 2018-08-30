#pragma once
#include <Core\2D\Components\Visible2DComponent.h>
class TextureBase;
class TexturedSpriteComponent : public Visible2DComponent
{
private:
	TextureBase * texture;

public:
	TexturedSpriteComponent();
	~TexturedSpriteComponent();

	virtual void PreRender()override;
	virtual void Render()override;
	virtual void PostRender()override;
};

