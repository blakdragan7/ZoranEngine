#pragma once
#include <Core/2D/SceneObject2D.h>
#include "Math/Vector2.h"

#include <Resources/ImageResource.h>

class TexturedSpriteComponent;
class ZoranEngine_EXPORT TexturedSprite : public SceneObject2D
{
protected:
	TexturedSpriteComponent * spriteComponent;

public:
	TexturedSprite(unsigned renderLayer, std::string name);
	TexturedSprite(unsigned renderLayer, std::string name, const char* texture);
	virtual ~TexturedSprite();

	void SetTexture(const char* path);
	void SetTexture(ImageResource texture);

};

