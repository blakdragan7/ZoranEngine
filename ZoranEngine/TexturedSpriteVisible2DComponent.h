#pragma once
#include <Core\2D\Components\Visible2DComponent.h>
class TextureBase;
class TexturedSpriteVisible2DComponent :
	public Visible2DComponent
{
private:
	TextureBase * texture;

public:
	TexturedSpriteVisible2DComponent();
	~TexturedSpriteVisible2DComponent();

	virtual void PreRender()override;
	virtual void Render()override;
	virtual void PostRender()override;
};

